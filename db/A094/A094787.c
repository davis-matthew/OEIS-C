/*
 * A094787  a(n) = smallest prime p such that p + n is a perfect power m^k, k
 *          >= 2.
 * https://oeis.org/A094787
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least prime p such that p + n is a perfect power
 *            (exponent k ≥ 2)
 * Range:     u8 n <= 27, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   compare_pow_seq, is_perfect_power, is_prime, kth_root_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include <limits.h>
#include "../utils/is_prime.h"
#include "../utils/compare_pow_seq.h"
#include "../utils/is_perfect_power.h"
#include "../utils/kth_root_floor.h"

/* Maximum prime to sieve initially. Adjust if larger p are needed. */
#define PRIME_LIMIT 5000000

/* Global storage for primes */
static uint32_t *primes = NULL;
static size_t prime_cnt = 0;
static bool primes_initialized = false;

/* Initialise prime table once */
static void init_primes(void)
{
    if (primes_initialized)
        return;

    /* Allocate sieve */
    bool *sieve = (bool *)calloc(PRIME_LIMIT + 1, sizeof(bool));
    if (!sieve) exit(EXIT_FAILURE);

    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        sieve[i] = true;

    for (size_t i = 2; i * i <= PRIME_LIMIT; ++i)
        if (sieve[i])
            for (size_t j = i * i; j <= PRIME_LIMIT; j += i)
                sieve[j] = false;

    /* Count primes */
    prime_cnt = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) ++prime_cnt;

    /* Allocate array and store primes */
    primes = (uint32_t *)malloc(prime_cnt * sizeof(uint32_t));
    if (!primes) exit(EXIT_FAILURE);

    size_t idx = 0;
    for (size_t i = 2; i <= PRIME_LIMIT; ++i)
        if (sieve[i]) primes[idx++] = (uint32_t)i;

    free(sieve);
    primes_initialized = true;
}

/* The required function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* undefined for n = 0, return 0 */
    init_primes();

    /* Search primes within the sieve first */
    for (size_t i = 0; i < prime_cnt; ++i)
    {
        seq_t p = (seq_t)primes[i];
        seq_t x = (seq_t)n + p;
        if (is_perfect_power(x))
            return p;
    }

    /* If not found, continue with larger primes */
    idx_t cand = PRIME_LIMIT + 1;
    if ((cand & 1) == 0) cand++;   /* make odd */

    while (1)
    {
        if (is_prime(cand))
        {
            seq_t x = (seq_t)n + (seq_t)cand;
            if (is_perfect_power(x))
                return (seq_t)cand;
        }
        cand += 2;   /* next odd number */
    }
}
