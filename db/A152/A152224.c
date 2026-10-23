/*
 * A152224  a(n)=4*a(n-1)+6*a(n-2), n>1 ; a(0)=1, a(1)=6 .
 * https://oeis.org/A152224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(1) = 6, a(n) = 4*a(n-1) + 6*a(n-2) for n > 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Cache previous terms so that calls in any order are fast. */
    static seq_t *cache = NULL;   /* Pointer to the dynamic array   */
    static idx_t cache_len = 0;   /* Currently stored number of terms */

    /* First call: allocate space for a(0) and a(1) */
    if (cache_len == 0) {
        cache = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!cache) exit(1);               /* allocation failed */
        cache[0] = (seq_t)1;               /* a(0) */
        cache[1] = (seq_t)6;               /* a(1) */
        cache_len = 2;
    }

    /* If the requested index is already cached, return immediately. */
    if (n < cache_len)
        return cache[n];

    /* Grow the cache to at least n+1 elements.  Double the size each time
       to keep amortised cost O(1) per term. */
    idx_t new_cap = cache_len;
    while (new_cap <= n) {
        /* Avoid infinite loop if new_cap overflows; cap at max size */
        if (new_cap == (idx_t)-1) break;
        new_cap *= 2;
    }

    seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                /* allocation failed */
    cache = tmp;

    /* Compute new terms using the recurrence relation.  The indices
       run from the old cache_len up to n inclusive. */
    for (idx_t i = cache_len; i <= n; ++i) {
        cache[i] = (seq_t)4 * cache[i-1] + (seq_t)6 * cache[i-2];
    }

    /* Update the cache length. */
    cache_len = n + 1;

    return cache[n];
}
