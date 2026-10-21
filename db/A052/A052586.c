/*
 * A052586  Expansion of e.g.f.: (1-x^3)/(1-x-x^3).
 * https://oeis.org/A052586
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0)=1, a(1)=1, a(2)=2, a(3)=6; For n≥4, a(n) = n*a(n-1) +
 *            n*(n-1)*(n-2)*a(n-3) (n in idx_t)
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 11, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Static cache of previously computed values */
    static seq_t *seq = NULL;      /* pointer to array of seq_t */
    static size_t sz  = 0;         /* number of initialized terms */
    static size_t cap = 0;         /* allocated capacity in seq_t elements */

    /* Initialize cache with the first four terms on first call */
    if (seq == NULL) {
        cap = 8;                   /* start with a moderate size */
        sz  = 4;
        seq = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!seq) return 0;        /* allocation failed */
        seq[0] = (seq_t)1;
        seq[1] = (seq_t)1;
        seq[2] = (seq_t)2;
        seq[3] = (seq_t)6;
    }

    /* Grow cache if the requested index is beyond its current size */
    if ((size_t)n >= sz) {
        /* Expand capacity to accommodate n+1 elements */
        size_t newcap = cap;
        if (newcap == 0) newcap = 8;
        while (newcap <= (size_t)n) {
            newcap *= 2;
            if (newcap <= (size_t)n) newcap = (size_t)n + 1;
        }
        seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        if (!tmp) return 0;           /* allocation failed */
        seq = tmp;
        cap = newcap;

        /* Compute all new terms using the recurrence */
        for (idx_t i = sz; i <= n; ++i) {
            if (i < 4) continue;     /* base terms already set */

            seq_t idx = (seq_t)i;
            seq_t term1 = idx * seq[i - 1];
            seq_t term2 = idx * (seq_t)(i - 1) * (seq_t)(i - 2) * seq[i - 3];
            seq[i] = term1 + term2;
        }
        sz = (size_t)n + 1;
    }

    return seq[n];
}
