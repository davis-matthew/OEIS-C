/*
 * A022127  Fibonacci sequence beginning 3, 17.
 * https://oeis.org/A022127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = a(n-1) + a(n-2), a(0) = 3, a(1) = 17
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdlib.h>

/* The sequence values are stored in a static cache so that
 * a(n) can be returned in O(1) time after an initial
 * O(n) expansion.  The cache is grown to at least n+1
 * elements whenever it is too small. */
static seq_t *cache = NULL;       /* array of stored values   */
static size_t cache_cap = 0;      /* total capacity of the array */
static idx_t  cache_size = 0;     /* number of values currently filled (>=2 after first init) */

seq_t a(idx_t n)
{
    /* Ensure the cache can hold indices up to n. */
    if (n + 1 > cache_cap) {
        size_t new_cap = cache_cap ? cache_cap * 2 : 1;
        while (new_cap <= (size_t)n)
            new_cap *= 2;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        cache = tmp;
        cache_cap = new_cap;
    }

    /* Initialise the cache on the first call. */
    if (cache_size == 0) {
        /* Store the two seed values. */
        cache[0] = (seq_t)3;
        if (cache_cap < 2) {
            seq_t *tmp = (seq_t *)realloc(cache, 2 * sizeof(seq_t));
            cache = tmp;
            cache_cap = 2;
        }
        cache[1] = (seq_t)17;
        cache_size = 2;
    }

    /* Compute missing values up to n (if any). */
    if (n >= cache_size) {
        for (idx_t i = cache_size; i <= n; ++i) {
            cache[i] = cache[i - 1] + cache[i - 2];
        }
        cache_size = n + 1;
    }

    return cache[n];
}
