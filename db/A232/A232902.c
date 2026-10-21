/*
 * A232902  Number of (n+1) X (2+1) 0..2 arrays with every element next to
 *          itself plus and minus one within the range 0..2 horizontally or
 *          antidiagonally.
 * https://oeis.org/A232902
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*a(n-1) - a(n-2) + 26*a(n-3) + 16*a(n-4) with a(1)=10,
 *            a(2)=34, a(3)=194, a(4)=1034 (n≥5)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 13, u64 n <= 27, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
# include <cstdlib>   // for abort()
# include <cstddef>   // for size_t
#else
# include <stdlib.h>  // for abort(), realloc()
# include <stddef.h>
#endif

seq_t a(idx_t n)
{
    /* static cache for memoisation */
    static seq_t *cache     = NULL;
    static size_t cap       = 0;   /* current allocated capacity */
    static size_t len       = 0;   /* highest n computed so far (=len) */

    if (n < 1) return 0;   /* undefined for the offset */

    /* Return cached value if already computed */
    if (n <= len) return cache[n-1];

    /* Ensure capacity for up to n terms */
    if (cap < n) {
        size_t new_cap = cap ? cap*2 : 16;
        while (new_cap < n) new_cap <<= 1;

#if defined(__cplusplus)
        seq_t *tmp = new seq_t[new_cap];
        if (cache) {
            for (size_t i = 0; i < len; ++i) tmp[i] = cache[i];
            delete[] cache;
        }
        cache = tmp;
        cap   = new_cap;
#else
        seq_t *tmp = (seq_t*)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) abort();          /* memory error */
        cache = tmp;
        cap   = new_cap;
#endif
    }

    /* Compute missing terms up to n */
    for (idx_t i = len + 1; i <= n; ++i) {
        seq_t val;
        if (i == 1) {
            val = 10;
        } else if (i == 2) {
            val = 34;
        } else if (i == 3) {
            val = 194;
        } else if (i == 4) {
            val = 1034;
        } else {
            /* recurrence: a(i) = 4*a(i-1) - a(i-2) + 26*a(i-3) + 16*a(i-4) */
            seq_t a_im1 = cache[i-2]; /* a(i-1) */
            seq_t a_im2 = cache[i-3]; /* a(i-2) */
            seq_t a_im3 = cache[i-4]; /* a(i-3) */
            seq_t a_im4 = cache[i-5]; /* a(i-4) */
            val = 4 * a_im1 - a_im2 + 26 * a_im3 + 16 * a_im4;
        }
        cache[i-1] = val;
    }
    len = n;
    return cache[n-1];
}
