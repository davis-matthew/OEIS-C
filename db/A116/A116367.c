/*
 * A116367  Sums of rows of the triangle in A116366.
 * https://oeis.org/A116367
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*prime(n+1) + sum_{k=1..n} prime(k) - 2 * * This
 *            implementation generates the first (n+1) primes using a simple *
 *            sieve of Eratosthenes and then evaluates the formula. The sieve
 *            * is regenerated each call if the required number of primes has
 *            * not been obtained yet. It uses only stand...
 * Range:     u8 n <= 7, u16 n <= 92, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include <math.h>

/* --------------------------------------------------------------------- */
/* Simple sieve: returns array of primes <= limit, *cnt_out is size   */
/* --------------------------------------------------------------------- */
static uint64_t *sieve_primes_upto(uint64_t limit, size_t *cnt_out)
{
    if (limit < 2) {
        *cnt_out = 0;
        return NULL;
    }
    /* Allocate primality bit‑array */
    bool *is_prime = (bool *)calloc(limit + 1, sizeof(bool));
    if (!is_prime) {
        *cnt_out = 0;
        return NULL;
    }
    for (uint64_t i = 2; i <= limit; ++i) {
        is_prime[i] = true;
    }

    uint64_t root = (uint64_t)sqrt((double)limit);
    for (uint64_t p = 2; p <= root; ++p) {
        if (is_prime[p]) {
            for (uint64_t j = p * p; j <= limit; j += p) {
                is_prime[j] = false;
            }
        }
    }

    /* Count primes */
    size_t cnt = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) ++cnt;
    }

    /* Allocate and fill prime list */
    uint64_t *primes = (uint64_t *)malloc(cnt * sizeof(uint64_t));
    if (!primes) {
        free(is_prime);
        *cnt_out = 0;
        return NULL;
    }
    size_t idx = 0;
    for (uint64_t i = 2; i <= limit; ++i) {
        if (is_prime[i]) primes[idx++] = i;
    }

    free(is_prime);
    *cnt_out = cnt;
    return primes;
}

/* --------------------------------------------------------------------- */
/* Sequence function a(n)                                              */
/* --------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* undefined at n < 1 */

    /* Number of primes needed = n+1 (for prime(n+1)) */
    size_t required = (size_t)(n + 1);
    size_t limit = 100;

    /* Find a limit that yields enough primes */
    while (1) {
        size_t prime_count;
        uint64_t *primes = sieve_primes_upto((uint64_t)limit, &prime_count);

        if (primes && prime_count >= required) {
            /* Sum first n primes: primes[0] .. primes[n-1] */
            seq_t sum = 0;
            for (size_t i = 0; i < (size_t)n; ++i) {
                sum += (seq_t)primes[i];
            }

            /* (n+1)-th prime is primes[n] */
            seq_t p_n_plus_1 = (seq_t)primes[n];

            seq_t result = ((seq_t)(n + 1)) * p_n_plus_1 + sum - (seq_t)2;

            free(primes);
            return result;
        }

        /* Not enough primes: enlarge the limit and try again */
        if (primes) free(primes);
        limit *= 2;
    }
}
