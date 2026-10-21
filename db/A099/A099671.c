/*
 * A099671  Partial sums of repdigits of A002278.
 * https://oeis.org/A099671
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn, easy
 * Formula:   a(n) = (4/81)*(10^(n+1) - 9*n - 10) (n ≥ 1) Alternative
 *            recurrence: a(1) = 4, a(2) = 48, a(3) = 492 a(n) = 12*a(n-1) -
 *            21*a(n-2) + 10*a(n-3) (n > 3) This implementation uses the
 *            recurrence with memoisation to answer any query in O(1) after
 *            the needed values have been computed
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 9, u64 n <= 19, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of computed values; index 0 unused */
    static seq_t *cache     = NULL;  /* terms 0 .. max_n */
    static idx_t cache_len  = 0;    /* allocated length */
    static idx_t max_comp   = 0;    /* highest n with value in cache */

    if (n < 1)
        return 0;   /* offset is 1; below offset values are irrelevant */

    /* initialise cache with first three terms, if not yet present */
    if (cache == NULL) {
        /* allocate indices 0..3 (size 4) */
        cache_len = 4;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        if (!cache) exit(1);
        cache[0] = 0;                 /* unused */
        cache[1] = 4;                 /* a(1) */
        cache[2] = 48;                /* a(2) */
        cache[3] = 492;               /* a(3) */
        max_comp = 3;
    }

    /* extend cache if required */
    if (n > max_comp) {
        /* ensure space up to index n */
        if (n + 1 > cache_len) {
            /* grow to at least n+1, double growth to reduce reallocs */
            idx_t new_len = n + 1;
            if (new_len < cache_len * 2) new_len = cache_len * 2;
            seq_t *tmp = (seq_t *)realloc(cache, new_len * sizeof(seq_t));
            if (!tmp) exit(1);
            cache = tmp;
            cache_len = new_len;
        }
        /* compute missing terms */
        for (idx_t i = max_comp + 1; i <= n; ++i) {
            seq_t term = cache[i - 1] * 12;          /* 12 * a(i-1) */
            term += cache[i - 3] * 10;           /* + 10 * a(i-3) */
            term -= cache[i - 2] * 21;           /* - 21 * a(i-2) */
            cache[i] = term;
        }
        max_comp = n;
    }

    return cache[n];
}
