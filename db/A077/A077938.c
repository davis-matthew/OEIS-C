/*
 * A077938  Expansion of 1/(1 - 2*x - x^2 - 2*x^3).
 * https://oeis.org/A077938
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = 2*a(n-1) + a(n-2) + 2*a(n-3), initial values: a(0) = 1,
 *            a(1) = 2, a(2) = 5. The recurrence applies for n ≥ 3
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms. */
    static seq_t *cache   = NULL;   /* a(0)..a(cache_sz-1) */
    static idx_t cache_sz = 0;      /* number of stored elements */

    /* If we already have the requested term, return it. */
    if (n < cache_sz)
        return cache[n];

    /* Need to enlarge cache up to n+1 elements. */
    idx_t new_sz = n + 1;
    seq_t *tmp;

    if (cache == NULL) { /* first allocation */
        tmp = (seq_t *)malloc(sizeof(seq_t) * new_sz);
        if (!tmp) return (seq_t)0;          /* allocation failed */
        tmp[0] = (seq_t)1;
        if (new_sz > 1) tmp[1] = (seq_t)2;
        if (new_sz > 2) tmp[2] = (seq_t)5;
        /* Compute any terms beyond the third if needed. */
        for (idx_t k = 3; k < new_sz; ++k)
            tmp[k] = (seq_t)2 * tmp[k-1] + tmp[k-2] + (seq_t)2 * tmp[k-3];
        cache = tmp;
        cache_sz = new_sz;
    } else {
        /* Resize existing array. */
        tmp = (seq_t *)realloc(cache, sizeof(seq_t) * new_sz);
        if (!tmp) return (seq_t)0;          /* allocation failed */
        /* If we are extending, compute only the new entries. */
        for (idx_t k = cache_sz; k < new_sz; ++k) {
            if (k < 3) { /* safety, though this should not happen after init */
                if (k == 0) tmp[0] = (seq_t)1;
                else if (k == 1) tmp[1] = (seq_t)2;
                else if (k == 2) tmp[2] = (seq_t)5;
            } else
                tmp[k] = (seq_t)2 * tmp[k-1] + tmp[k-2] + (seq_t)2 * tmp[k-3];
        }
        cache = tmp;
        cache_sz = new_sz;
    }

    return cache[n];
}
