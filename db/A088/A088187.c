/*
 * A088187  Primes that are a sum of twin primes and their indices.
 * https://oeis.org/A088187
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = the n-th prime that can be expressed as p = q + r +
 *            index(q) + index(r), where (q, r) is a twin prime pair (i.e., r
 *            = q + 2) and index(x) is the 1‑based position of prime x in the
 *            ordered list of all primes
 * Range:     u8 n <= 2, u16 n <= 41, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   is_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Required utilities */
#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_prime.h"
#include "../utils/is_prime_big.h"

/* Cache for previously computed terms */
static seq_t *cache = NULL;
static size_t cache_len = 0;
static size_t cache_cap = 0;

/* Current state of prime generation */
static idx_t next_candidate = 2;   /* next integer to test for primality */
static idx_t prime_index   = 0;    /* number of primes found so far */
static idx_t last_prime    = 0;    /* last prime encountered */
static idx_t last_index    = 0;    /* its index */

/* Append a value to the cache, growing it if necessary. */
static void cache_append(seq_t val)
{
    if (cache_len == cache_cap) {
        size_t new_cap = cache_cap ? cache_cap << 1 : 8;
        /* Cast the result of realloc to seq_t* to satisfy C++ */
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();        /* memory failure */
        cache = tmp;
        cache_cap = new_cap;
    }
    cache[cache_len++] = val;
}

/* Ensure that at least n_needed terms are stored in the cache. */
static void ensure_cache(idx_t n_needed)
{
    if (cache_len >= n_needed)
        return;

    /* Generate primes incrementally until we have enough terms. */
    while (cache_len < n_needed) {
        /* Handle 2 separately (only odd numbers thereafter). */
        if (next_candidate == 2) {
            prime_index++;
            last_prime = 2;
            last_index = prime_index;
            next_candidate = 3;
            continue;
        }

        /* Test the current candidate for primality. */
        if (is_prime(next_candidate)) {
            prime_index++;

            if (last_prime != 0 && next_candidate - last_prime == 2) {
                /* We have a twin pair (last_prime, next_candidate). */
                seq_t sum =
                    (seq_t)next_candidate
                  + (seq_t)last_prime
                  + (seq_t)prime_index
                  + (seq_t)last_index;

                if (is_prime_big(sum))
                    cache_append(sum);
            }

            last_prime = next_candidate;
            last_index = prime_index;
        }

        /* Advance to the next odd candidate. */
        next_candidate += 2;
    }
}

/* Public sequence function. */
seq_t a(idx_t n)
{
    if (n == 0)          /* behaviour for n < 1 is undefined */
        return (seq_t)0;

    ensure_cache(n);
    return cache[n - 1];
}
