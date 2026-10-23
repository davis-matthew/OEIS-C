/*
 * A135231  Row sums of triangle A135230.
 * https://oeis.org/A135231
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence: Row sums of triangle A135230 (OEIS A135231)
   Formula: a(0) = 1, a(1) = 2, a(2) = 4, a(3) = 6
            a(n) = 2*a(n-1) + a(n-2) - 2*a(n-3)   for n > 3
   This recurrence is linear of order 3 and generates all terms of the
   sequence.  A memoised array is used so that any call order is handled
   efficiently.
*/

#include <stdlib.h>
#include <stddef.h>

static seq_t *cache = NULL;       /* memoisation array */
static size_t cache_size = 0;     /* number of elements currently stored */
static size_t cache_cap  = 0;     /* allocated capacity of cache */

/* Return the n-th term of sequence A135231. */
seq_t a(idx_t n)
{
    /* Ensure enough storage for index n. */
    if (cache_size == 0) {
        cache_cap = (n >= 4 ? (size_t)n + 1 : 4);
        cache = (seq_t *)malloc(cache_cap * sizeof(seq_t));
    } else if ((size_t)n >= cache_cap) {
        size_t new_cap = cache_cap * 2;
        while ((size_t)n >= new_cap) new_cap *= 2;
        cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache_cap = new_cap;
    }
    if (!cache) return (seq_t)0; /* allocation failure, fallback value */

    /* If we have not yet initialised the base cases, do so. */
    if (cache_size == 0) {
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)2;
        cache[2] = (seq_t)4;
        cache[3] = (seq_t)6;
        cache_size = 4;
    }

    /* If request is for an index already stored, return immediately. */
    if ((size_t)n < cache_size) {
        return cache[(size_t)n];
    }

    /* Compute subsequent terms using recurrence, up to index n. */
    for (idx_t k = (idx_t)cache_size; k <= n; ++k) {
        if (k <= 3) continue; /* base values already known */
        seq_t val = 2 * cache[k - 1] + cache[k - 2] - 2 * cache[k - 3];
        cache[k] = val;
    }
    cache_size = (size_t)n + 1;
    return cache[(size_t)n];
}
