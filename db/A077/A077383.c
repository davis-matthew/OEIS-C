/*
 * A077383  a(1) = 1, then add, multiply and subtract 2, 3, 4; 5, 6, 7; ... in
 *          that order.
 * https://oeis.org/A077383
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(1) = 1; for m ≥ 1 let c = (m‑1)/3 // integer division if m mod
 *            3 == 1: a(m+1) = a(m) + (2 + 3*c) if m mod 3 == 2: a(m+1) = a(m)
 *            * (3 + 3*c) if m mod 3 == 0: a(m+1) = a(m) - (4 + 3*c) (This
 *            matches the description 'add, multiply, subtract 2,3,4; 5,6,7;
 *            …')
 * Range:     u8 n <= 8, u16 n <= 14, u32 n <= 26, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* static cache: a[1] holds the first term */
    static seq_t *cache = NULL;
    static idx_t cache_cap = 0;   /* allocated capacity (≥ n+1) */
    static idx_t cache_filled = 0;/* highest index computed so far */

    /* initialise on first call */
    if (cache == NULL) {
        cache_cap = 8;                               /* initial capacity */
        cache = (seq_t *)malloc((cache_cap + 1) * sizeof(seq_t));
        cache[1] = (seq_t)1;                         /* a(1) = 1 */
        cache_filled = 1;
    }

    /* enlarge cache if needed */
    if (n > cache_cap) {
        idx_t new_cap = cache_cap;
        while (new_cap < n) new_cap <<= 1;          /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!tmp) return (seq_t)0;                     /* fallback on OOM */
        cache = tmp;
        cache_cap = new_cap;
    }

    /* compute terms up to n if not yet cached */
    for (idx_t m = cache_filled; m < n; ++m) {
        seq_t prev = cache[m];
        idx_t c = (m - 1) / 3;           /* group index */
        seq_t t;
        if (m % 3 == 1) {                /* add */
            t = (seq_t)(2 + 3 * c);
            cache[m + 1] = prev + t;
        } else if (m % 3 == 2) {         /* multiply */
            t = (seq_t)(3 + 3 * c);
            cache[m + 1] = prev * t;
        } else {                         /* subtract */
            t = (seq_t)(4 + 3 * c);
            cache[m + 1] = prev - t;
        }
    }
    cache_filled = n;
    return cache[n];
}
