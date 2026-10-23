/*
 * A192346  Constant term of the reduction (by x^2->x+1) of polynomial p(n,x)
 *          identified in Comments.
 * https://oeis.org/A192346
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*a(n-1)+6*a(n-2)-2*a(n-3)-a(n-4) initial values: a(1)=1,
 *            a(2)=0, a(3)=3, a(4)=4
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 20, u64 n <= 27, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache for memoization (1‑based indexing) */
    static seq_t *cache = NULL;
    static idx_t cap   = 0; /* allocated size (excluding index 0) */
    static idx_t cur   = 0; /* largest n whose value is stored */

    /* grow cache to hold all requested indices */
    if (cap < n)
    {
        size_t new_cap = cap ? cap * 2 : 16;
        while (new_cap < n) new_cap *= 2;
        seq_t *p = (seq_t *)realloc(cache, (new_cap + 1) * sizeof(seq_t));
        if (!p) return 0;  /* allocation failure: return 0 or abort */
        cache = p;
        cap   = new_cap;
    }

    /* initialise first four values if they are not yet stored */
    if (cur < 4)
    {
        cache[1] = (seq_t)1;
        cache[2] = (seq_t)0;
        cache[3] = (seq_t)3;
        cache[4] = (seq_t)4;
        cur = 4;
    }

    /* compute any missing terms up to n */
    for (idx_t i = cur + 1; i <= n; ++i)
    {
        seq_t v = 2 * cache[i - 1]          /* 2*a(i-1)   */
                + 6 * cache[i - 2]          /* 6*a(i-2)   */
                - 2 * cache[i - 3]          /* -2*a(i-3)  */
                - cache[i - 4];             /* -a(i-4)    */
        cache[i] = v;
    }
    cur = n;
    return cache[n];
}
