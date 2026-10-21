/*
 * A088767  a(n) = A087697(n)/2.
 * https://oeis.org/A088767
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 28, u16 n <= 56, u32 n <= 56, u64 n <= 56, u128 n <= 56,
 *            big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: numbers n such that 2n-7 and 2n+7 are both prime.
   a(n) returns the n‑th such integer (1‑based indexing). */

#include <stdbool.h>
#include <stdlib.h>
#include "../utils/is_prime.h"

static seq_t *cache = NULL;          /* stored terms of the sequence   */
static size_t cache_cap = 0;         /* current capacity of cache     */
static size_t cache_len = 0;         /* number of terms cached        */
static idx_t next_candidate = 5;     /* next integer to test for the pair of primes */

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* out of bounds, return 0 */

    /* Extend the cache until we have at least n terms. */
    while (cache_len < (size_t)n)
    {
        /* Ensure a large enough cache, grow geometrically. */
        if (cache_len >= cache_cap)
        {
            cache_cap = cache_cap ? cache_cap * 2 : 16;
            cache = (seq_t *)realloc(cache, cache_cap * sizeof(seq_t));
        }

        idx_t m = next_candidate;
        idx_t p1 = m * 2 - 7;
        idx_t p2 = m * 2 + 7;

        /* Test the two primes.  First test the smaller one for speed. */
        if (is_prime(p1) && is_prime(p2))
        {
            cache[cache_len++] = (seq_t)m;
        }

        ++next_candidate;
    }

    return cache[n - 1];
}
