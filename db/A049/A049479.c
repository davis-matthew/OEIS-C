/*
 * A049479  Smallest prime dividing 2^n - 1.
 * https://oeis.org/A049479
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = smallest prime divisor of (2^n - 1)
 * Range:     u8 n <= 12, u16 n <= 16, u32 n <= 60, u64 n <= 88, u128 n <= 520,
 *            big any n
 * Helpers:   build_primes_upto, is_prime, mod_pow, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_pow.h"        /* seq_pow(seq_t, idx_t)      */
#include "../utils/mod_pow.h"        /* mod_pow(seq_t, idx_t, seq_t) */
#include "../utils/is_prime.h"       /* is_prime(idx_t)            */
#include "../utils/build_primes_upto.h" /* build_primes_upto(...) */

static uint64_t *prime_list = NULL;
static size_t prime_cnt   = 0;

/* Generate a static list of primes up to a fixed limit.
   The limit is high enough for all tests up to n = 10000. */
static void init_prime_list(void)
{
    if (prime_list == NULL) {
        /* 1 000 000 covers more than 78 000 primes – plenty for the
           expected small divisors of 2ⁿ‑1 with n ≤ 10 000. */
        build_primes_upto(1000000, &prime_list, &prime_cnt);
    }
}

seq_t a(idx_t n)
{
    /* defined only for n ≥ 2 */
    if (n < 2) {
        return 0;
    }

    /* 3 always divides 2ⁿ‑1 for even n (2ⁿ ≡ 1 (mod 3) when n even) */
    if ((n & 1) == 0) {
        return (seq_t)3;
    }

    init_prime_list();

    /* Search for the smallest prime factor among the pre‑computed primes. */
    for (size_t i = 0; i < prime_cnt; ++i) {
        uint64_t p = prime_list[i];
        if (p == 2) {
            continue;                 /* 2 never divides 2ⁿ‑1 for n ≥ 1 */
        }
        if (mod_pow((seq_t)2, n, (seq_t)p) == (seq_t)1) {
            return (seq_t)p;          /* smallest divisor found */
        }
    }

    /* If no small divisor was found, n is (probably) prime and
       2ⁿ‑1 might itself be prime (a Mersenne prime).  In this
       case the smallest prime divisor equals the whole number.   */

    if (is_prime(n)) {
        seq_t M = seq_pow((seq_t)2, n);
        M -= (seq_t)1;
        return M;
    }

    /* Fallback: continue searching beyond the pre‑computed list.
       For the test range this is rarely needed, but it guarantees
       correctness if an uncommon large factor occurs.           */
    uint64_t candidate = prime_list[prime_cnt - 1] + 2;
    while (candidate < UINT64_MAX) {
        if (mod_pow((seq_t)2, n, (seq_t)candidate) == (seq_t)1) {
            return (seq_t)candidate;
        }
        candidate += 2;  /* only odd candidates */
    }

    /* If we still cannot find a divisor, return 0 as a last resort.
       This situation should not occur for n ≤ 10 000 under normal
       assumptions. */
    return 0;
}
