/*
 * A096046  a(n) = B(2n,3)/B(2n) (see comment).
 * https://oeis.org/A096046
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1/4)*(7*9^n - 3) (equivalently a(n)=9*a(n-1)+6, a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 40,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *cache   = NULL;   /* pointer to allocated array */
    static idx_t  comp    = 0;      /* number of terms that are already computed (cache[0..comp-1] valid) */
    static idx_t  capacity = 0;     /* current size of allocated array */

    /* initialise on first call */
    if (cache == NULL) {
        capacity = 1;
        cache = (seq_t *)malloc(capacity * sizeof(seq_t));
        if (!cache) abort();                 /* allocation failed */
        cache[0] = (seq_t)1;                 /* a(0)=1 */
        comp = 1;
    }

    /* grow the array if necessary; use doubling to keep amortised cost O(1) */
    if (n >= capacity) {
        idx_t new_cap = capacity;
        while (new_cap <= n) {
            new_cap = new_cap ? new_cap * 2 : 1;
        }
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();                   /* reallocation failed */
        cache = tmp;
        capacity = new_cap;
    }

    /* compute any missing terms up to n */
    if (n >= comp) {
        for (idx_t i = comp; i <= n; ++i) {
            /* recurrence: a(i) = 9*a(i-1) + 6 */
            cache[i] = cache[i-1] * (seq_t)9 + (seq_t)6;
        }
        comp = n + 1;
    }

    return cache[n];
}
