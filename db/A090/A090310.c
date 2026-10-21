/*
 * A090310  a(n) = 21*a(n-1) + a(n-2), starting with a(0) = 2 and a(1) = 21.
 * https://oeis.org/A090310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 21*a(n-1) + a(n-2), a(0) = 2, a(1) = 21
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 14, u128 n <= 29,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This function keeps a cache in order to answer calls in any order. */

#include <stdlib.h>

/* Cache of computed terms; grows as needed */
static seq_t *a_cache = NULL;   /* pointer to cached values   */
static idx_t  a_cache_len = 0;  /* number of stored terms     */

seq_t a(idx_t n)
{
    /* Ensure the first two terms are cached */
    if (a_cache_len == 0) {
        a_cache_len = 2;
        a_cache = (seq_t *)realloc(NULL, a_cache_len * sizeof(seq_t));
        a_cache[0] = (seq_t)2;
        a_cache[1] = (seq_t)21;
    }

    /* If requested term is already cached, return it immediately */
    if (n < a_cache_len) {
        return a_cache[n];
    }

    /* Grow the cache to hold up to term n */
    a_cache = (seq_t *)realloc(a_cache, (n + 1) * sizeof(seq_t));
    for (idx_t i = a_cache_len; i <= n; ++i) {
        /* Recurrence: a[i] = 21*a[i-1] + a[i-2] */
        a_cache[i] = (seq_t)21 * a_cache[i-1] + a_cache[i-2];
    }
    a_cache_len = n + 1;

    return a_cache[n];
}
