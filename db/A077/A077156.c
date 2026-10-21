/*
 * A077156  a(0) = 1, followed by next n primes followed by next n composite
 *          numbers.
 * https://oeis.org/A077156
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation of OEIS A077156
 *
 * a(0) = 1, then for m = 1,2,3,… append the next m primes followed by
 * the next m composite numbers.  The sequence is generated lazily and
 * cached so that
 *
 *     a(n) = a(n-1) + a(n-2) + … + a(0) + 1  (not a direct formula)
 *
 * but the algorithm above constructs the sequence directly.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Primitive 64‑bit primality test.
 * Works well for the range of values required for n ≤ 10 000. */
static bool isPrime(uint64_t n)
{
    if (n < 2) return false;
    if ((n & 1) == 0) return n == 2;
    for (uint64_t d = 3; d <= n / d; d += 2)
        if (n % d == 0) return false;
    return true;
}

/* Cached sequence data.  All values are stored as `seq_t`. */
static seq_t *cache      = NULL;   /* array of terms          */
static idx_t  cache_len  = 0;      /* number of cached terms  */
static idx_t  cache_cap  = 0;      /* allocated capacity      */

/* Generation state */
static uint64_t next_prime    = 2;  /* next prime candidate   */
static uint64_t next_composite = 4;/* next composite candidate*/
static idx_t    block_index   = 1;  /* current block size      */

/* Append a value to the cache, growing the array when needed. */
static void push_value(seq_t val)
{
    if (cache_len >= cache_cap) {
        cache_cap = cache_cap ? cache_cap * 2 : 16;
        seq_t *tmp = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        if (!tmp) abort();          /* safety on allocation failure */
        cache = tmp;
    }
    cache[cache_len++] = val;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    /* Initialise on first call */
    if (cache_len == 0) {
        cache_cap = 16;
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
        if (!cache) abort();             /* abort on allocation failure */
        cache[0] = (seq_t)1;             /* a(0) = 1 */
        cache_len = 1;
    }

    /* Generate terms up to the requested index */
    while (cache_len <= n) {
        idx_t m = block_index;  /* block size for this iteration */

        /* Generate the next m primes */
        for (idx_t i = 0; i < m; ++i) {
            while (!isPrime(next_prime))
                ++next_prime;
            push_value((seq_t)next_prime);
            ++next_prime;
        }

        /* Generate the next m composites */
        for (idx_t i = 0; i < m; ++i) {
            while (next_composite < 4 || isPrime(next_composite))
                ++next_composite;
            push_value((seq_t)next_composite);
            ++next_composite;
        }

        ++block_index;  /* advance to the next block */
    }

    return cache[n];
}
