/*
 * A170220  Number of reduced words of length n in Coxeter group on 19
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^40 = I.
 * https://oeis.org/A170220
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) is defined as the coefficient of t^n in (t^40 + 2*t^39 + …
 *            + 2*t + 1) / (153*t^40 – 17*t^39 – … – 17*t + 1). Equivalently,
 *            a(n) satisfies the recurrence a(n) = p(n) +
 *            17·∑_{k=1}^{min(39,n)} a(n–k) – (n≥40 ? 153·a(n–40) : 0) where
 *            p(0)=1, p(1)…p(39)=2, p(40)=1, and p(n)=0 for n>40. Initial
 *            va...
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>  /* for malloc, realloc */

seq_t a(idx_t n)
{
    /* static cache: a[0]..a[last] already computed */
    static seq_t *cache = NULL;
    static size_t size  = 0;          /* allocated length */
    static idx_t  last  = (idx_t)-1;  /* highest index computed */

    /* allocate/extend cache if necessary */
    if ((size_t)n >= size) {
        seq_t *tmp = (seq_t*)realloc(cache, (n + 1) * sizeof(seq_t));
        if (!tmp) abort();          /* out of memory, terminate */
        /* zero new entries */
        for (size_t i = size; i <= (size_t)n; ++i)
            tmp[i] = 0;
        cache = tmp;
        size  = n + 1;
    }

    /* initialise first term if cache empty */
    if (last == (idx_t)-1) {
        cache[0] = 1;
        last = 0;
    }

    /* compute missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i) {
        seq_t sum = 0;
        idx_t lim = i < 39 ? i : 39;
        for (idx_t k = 1; k <= lim; ++k)
            sum += cache[i - k];

        seq_t val;
        if (i == 0)
            val = 1;                    /* p(0) */
        else if (i <= 39)
            val = 2;                    /* p(1)…p(39) */
        else if (i == 40)
            val = 1;                    /* p(40) */
        else
            val = 0;                    /* p(n) = 0 for n>40 */

        val += (seq_t)17 * sum;          /* 17 * Σ a(n−k) */
        if (i >= 40)
            val -= (seq_t)153 * cache[i - 40];  /* −153·a(n−40) */

        cache[i] = val;
    }

    last = n;
    return cache[n];
}
