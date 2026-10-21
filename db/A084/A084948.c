/*
 * A084948  a(n) = Product_{i=0..n-1} (8*i+2).
 * https://oeis.org/A084948
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ∏_{i=0}^{n-1} (8*i+2) (a(0)=1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 12, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache for memoized pre‑computation */
    static seq_t *cache = NULL;   /* array of a(0), a(1), ... */
    static idx_t   cap   = 0;     /* allocated capacity */
    static idx_t   len   = 0;     /* current number of computed terms */

    /* Ensure sufficient capacity */
    if (n >= cap) {
        idx_t newcap = cap == 0 ? 16 : cap << 1;
        while (newcap <= n) newcap <<= 1;
        cache = (seq_t *)realloc(cache, newcap * sizeof(seq_t));
        cap = newcap;
    }

    /* Initialise cache[0] if not yet done */
    if (len == 0) {
        cache[0] = (seq_t)1;   /* a(0) = 1 */
        len = 1;
    }

    /* Compute missing terms up to n */
    for (idx_t i = len - 1; i < n; ++i) {
        seq_t factor = (seq_t)(8 * i + 2);   /* 8*i+2 fits in idx_t */
        cache[i + 1] = cache[i] * factor;    /* a(i+1) = a(i) * (8*i+2) */
    }
    len = n + 1;

    return cache[n];
}
