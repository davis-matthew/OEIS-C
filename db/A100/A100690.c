/*
 * A100690  a(n) = p * 5^p - 1 where p=prime(n).
 * https://oeis.org/A100690
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = p * 5^p - 1, where p = prime(n)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 9, u128 n <= 12,
 *            big any n
 * Helpers:   estimate_prime_upper_bound, pow_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/pow_seq.h"                     /* seq_t pow_seq(seq_t base, idx_t exp); */
#include "../utils/estimate_prime_upper_bound.h" /* uint64_t estimate_prime_upper_bound(idx_t n); */

static idx_t *prime_cache = NULL;    /* 1‑based array of primes. */
static size_t prime_cache_size = 0;   /* Number of primes stored. */

/* Return the n‑th prime, extending the cache if necessary. */
static idx_t get_prime(idx_t n)
{
    if (n == 0)
        return 0;   /* Undefined for n = 0. */

    /* Return cached prime if already computed. */
    if ((size_t)n <= prime_cache_size)
        return prime_cache[n - 1];

    /* Compute a sieve up to an upper bound. */
    uint64_t limit = estimate_prime_upper_bound(n) + 100ULL;   /* safety margin */
    uint8_t *sieve = (uint8_t *)calloc(limit + 1, sizeof(uint8_t));
    if (!sieve)
        return 0;   /* OOM – caller will get 0. */

    size_t new_cap = prime_cache_size ? prime_cache_size : 128;
    size_t new_size = prime_cache_size;
    if (!prime_cache)
        prime_cache = (idx_t *)malloc(new_cap * sizeof(idx_t));

    for (uint64_t i = 2; i <= limit; ++i) {
        if (!sieve[i]) {
            /* i is prime */
            if (new_size >= new_cap) {
                new_cap *= 2;
                idx_t *tmp = (idx_t *)realloc(prime_cache, new_cap * sizeof(idx_t));
                if (!tmp) {
                    free(sieve);
                    free(prime_cache);
                    return 0;
                }
                prime_cache = tmp;
            }
            prime_cache[new_size++] = (idx_t)i;

            if (i > limit / i)
                continue;
            for (uint64_t j = i * i; j <= limit; j += i)
                sieve[j] = 1;
        }
    }
    free(sieve);

    prime_cache_size = new_size;
    if ((size_t)n <= prime_cache_size)
        return prime_cache[n - 1];
    return 0;   /* Should not happen for sane inputs. */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;   /* Undefined for n < 1, return 0. */

    idx_t p = get_prime(n);
    if (p == 0)
        return (seq_t)0;   /* Failure to get prime. */

    seq_t pow = pow_seq((seq_t)5, p);   /* 5^p in seq_t */
    seq_t result = pow * (seq_t)p - (seq_t)1; /* p*5^p - 1 */
    return result;
}
