/*
 * A170862  a(0) = 1, a(1) = 0; a(2^i + j) = 3a(j) + 3a(j + 1) for 0 <= j <
 *          2^i.
 * https://oeis.org/A170862
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 61, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A170862
   a(0) = 1, a(1) = 0;
   a(2^i + j) = 3·a(j) + 3·a(j+1)      for 0 ≤ j < 2^i          */
#include <stdlib.h>

static seq_t *cache = NULL;   /* memoised values keyed by index */
static idx_t  cache_size = 0;

/* Return the n‑th term of the sequence.  The function builds values
   on demand and remembers all computed terms, so repeated or out‑of‑
   order calls are fast. */
seq_t a(idx_t n)
{
    /* Ensure that cache holds at least the first two values. */
    if (cache == NULL) {
        cache_size = 2;
        cache = (seq_t *)malloc(cache_size * sizeof(seq_t));
        if (cache == NULL) return (seq_t)0;   /* allocation failure */
        cache[0] = (seq_t)1;
        cache[1] = (seq_t)0;
    }

    /* If the requested index is already cached, return it. */
    if (n < cache_size) return cache[n];

    /* Grow the cache to accommodate indices up to n. */
    if (n >= cache_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *new_cache = (seq_t *)realloc(cache, new_size * sizeof(seq_t));
        if (new_cache == NULL) return (seq_t)0;   /* allocation failure */
        cache = new_cache;
        cache_size = new_size;
    }

    /* Compute missing values sequentially.  The recurrence is linear
       in n, so an O(n) single pass is sufficient. */
    idx_t pow = 1;   /* current highest power of two ≤ current index */
    for (idx_t k = 2; k <= n; ++k) {
        if (k == (pow << 1)) pow <<= 1;          /* advance to next power */
        idx_t j = k - pow;
        seq_t tmp = cache[j] + cache[j + 1];
        cache[k] = tmp * (seq_t)3;
    }

    return cache[n];
}
