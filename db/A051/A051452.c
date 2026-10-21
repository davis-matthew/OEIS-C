/*
 * A051452  a(n) = 1 + lcm(1..k) where k is the n-th prime power A000961(n).
 * https://oeis.org/A051452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   build_primes_upto, seq_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes a(n) = 1 + lcm(1..k) where k is the n‑th prime power (A000961) */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include <stddef.h>

#include "../utils/build_primes_upto.h"
#include "../utils/seq_mul.h"

/*--------------------------------------------------------------------
   Prime powers and prime list
--------------------------------------------------------------------*/
static uint64_t *prime_powers = NULL;   /* sorted list of prime powers   */
static size_t    pp_count    = 0;        /* number of elements            */
static uint64_t *primes      = NULL;    /* prime list                    */
static size_t    primes_cnt  = 0;        /* number of primes              */
static const uint64_t MAX_LIMIT = 2000000;   /* sufficient for n up to 10000 */

/* compare for qsort (uint64_t) */
static int cmp_uint64(const void *a, const void *b)
{
    uint64_t x = *(const uint64_t *)a;
    uint64_t y = *(const uint64_t *)b;
    return (x < y) ? -1 : (x > y);
}

/* One‑time initialization of primes and prime powers */
static void init_prime_power_list(void)
{
    static bool done = false;
    if (done) return;
    done = true;

    /* Build list of primes up to MAX_LIMIT */
    build_primes_upto(MAX_LIMIT, &primes, &primes_cnt);

    /* Reserve reasonable space for prime powers */
    size_t estimate = primes_cnt * 2;
    prime_powers = (uint64_t *)malloc(estimate * sizeof(uint64_t));
    size_t idx = 0;

    /* Include 1 as the first prime power */
    prime_powers[idx++] = 1;

    for (size_t i = 0; i < primes_cnt; ++i) {
        uint64_t p = primes[i];
        uint64_t val = p;
        while (val <= MAX_LIMIT) {
            if (idx == estimate) {
                estimate <<= 1;
                prime_powers = (uint64_t *)realloc(prime_powers,
                                                  estimate * sizeof(uint64_t));
            }
            prime_powers[idx++] = val;
            /* Prevent overflow before next multiplication */
            if ((unsigned __int128)val * p > MAX_LIMIT) break;
            val *= p;
        }
    }

    pp_count = idx;
    prime_powers = (uint64_t *)realloc(prime_powers, pp_count * sizeof(uint64_t));
    qsort(prime_powers, pp_count, sizeof(uint64_t), cmp_uint64);
}

/*--------------------------------------------------------------------
   Compute lcm(1..k) for a given k (k <= MAX_LIMIT)
--------------------------------------------------------------------*/
static seq_t lcm_upto_k(uint64_t k)
{
    seq_t result = 1;
    for (size_t i = 0; i < primes_cnt && primes[i] <= k; ++i) {
        uint64_t p = primes[i];
        uint64_t pw = 1;
        while (pw <= k / p) pw *= p;
        result = seq_mul(result, (seq_t)pw);
    }
    return result + (seq_t)1;   /* +1 as per the definition */
}

/*--------------------------------------------------------------------
   Public sequence function
--------------------------------------------------------------------*/
seq_t a(idx_t n)
{
    if (n == 0) return 0;                 /* undefined for offset 0 */
    init_prime_power_list();
    if ((idx_t)n > pp_count) return 0;    /* safety; should not happen */
    uint64_t k = prime_powers[n - 1];      /* nth prime power (1‑based) */
    return lcm_upto_k(k);
}
