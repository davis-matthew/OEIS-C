/*
 * A069533  a(n) = Sum_{p prime} floor(n^2/p^2) where the sum is over all the
 *          primes.
 * https://oeis.org/A069533
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Sum_{p prime} floor(n^2 / p^2)
 * Range:     u8 n <= 15, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Compute the nth term of OEIS A069533 for offset 1. */
seq_t a(idx_t n)
{
    /* static state for dynamic prime generation */
    static size_t prime_cnt = 0;          /* number of primes stored   */
    static size_t prime_cap = 0;          /* allocated capacity       */
    static uint32_t *primes = NULL;       /* array of primes          */
    static idx_t prime_limit = 0;         /* highest n for which primes were generated */

    /* Ensure we have all primes <= n.  Rebuild if necessary. */
    if (n > prime_limit) {
        /* Determine a safe allocation size (overkill is fine). */
        size_t new_cap = (size_t)n + 1;
        uint32_t *new_primes = (uint32_t *)realloc(primes, new_cap * sizeof(uint32_t));
        if (!new_primes) {
            /* If allocation fails, return 0 (should never happen with small n). */
            return 0;
        }
        primes = new_primes;
        prime_cap = new_cap;

        /* Simple sieve of Eratosthenes up to n. */
        uint8_t *is_prime = (uint8_t *)calloc((size_t)n + 1, 1);
        if (!is_prime) return 0;  /* allocation failure safeguard */

        for (uint32_t i = 2; i <= n; ++i) is_prime[i] = 1;
        for (uint32_t i = 2; (uint64_t)i * i <= n; ++i) {
            if (is_prime[i]) {
                for (uint32_t j = i * i; j <= n; j += i)
                    is_prime[j] = 0;
            }
        }

        prime_cnt = 0;
        for (uint32_t i = 2; i <= n; ++i) {
            if (is_prime[i]) {
                primes[prime_cnt++] = i;
            }
        }
        free(is_prime);
        prime_limit = n;
    }

    /* Compute the sum.  n^2 fits into seq_t for the intended ranges. */
    seq_t n_sq = (seq_t)n * (seq_t)n;
    seq_t sum = 0;
    for (size_t i = 0; i < prime_cnt; ++i) {
        seq_t p = (seq_t)primes[i];
        seq_t p_sq = p * p;
        sum += n_sq / p_sq;
    }
    return sum;
}
