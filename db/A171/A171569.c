/*
 * A171569  Triangular numbers T such that T-2 is a prime.
 * https://oeis.org/A171569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = triangular number T_k = k*(k+1)/2 such that T_k-2 is
 *            prime, offset 1 (a(1) is the first such triangular number)
 * Range:     u8 n <= 9, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

/* Static cache: cache[n-1] holds a(n). */
static seq_t *cache     = NULL;   /* cache[0] = a(1) */
static size_t cache_size = 0;     /* number of terms stored */
static size_t cache_cap   = 0;     /* allocated capacity */
static idx_t  processed_k = 0;    /* last k processed (k starts at 0) */

/* Ensure cache has at least 'needed' capacity. */
static void grow_cache(size_t needed)
{
    if (cache_cap >= needed)
        return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 16;
    while (new_cap < needed)
        new_cap <<= 1;
    seq_t *new_ptr = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (new_ptr)
    {
        cache = new_ptr;
        cache_cap = new_cap;
    }
}

/* Return the n-th term of the sequence. */
seq_t a(idx_t n)
{
    if (n == 0)                 /* offset is 1; undefined behavior below 1 */
        return (seq_t)0;

    /* Grow cache until it contains n terms. */
    while (cache_size < (size_t)n)
    {
        /* Next k to test. */
        ++processed_k;
        uint64_t k = (uint64_t)processed_k;

        /* Triangular number T_k = k*(k+1)/2.  Use 64‑bit arithmetic. */
        uint64_t tri = k * (k + 1) / 2;

        if (tri <= 2)           /* T-2 not positive */
            continue;

        /* Check if T_k-2 is prime. */
        if (is_prime((idx_t)(tri - 2)))
        {
            /* Store the term (truncated if seq_t cannot hold the full value). */
            grow_cache(cache_size + 1);
            cache[cache_size++] = (seq_t)tri;
        }
    }

    /* cache[0] corresponds to a(1). */
    return cache[n - 1];
}
