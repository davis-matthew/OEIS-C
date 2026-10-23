/*
 * A104187  Expansion of g.f. -(1+x^2+x^4)/((x^3+x^2+x-1)*(x-1)^2).
 * https://oeis.org/A104187
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0)=1, a(1)=3, a(2)=8, a(3)=18, a(4)=38,
 *            a(n)=3*a(n-1)-2*a(n-2)-a(n-4)+a(n-5) for n >= 5
 * Range:     u8 n <= 6, u16 n <= 15, u32 n <= 34, u64 n <= 70, u128 n <= 143,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Dynamic memoisation table for the recurrence */
    static seq_t *tbl  = NULL;   /* stored terms */
    static idx_t sz   = 0;       /* number of filled entries */
    static idx_t cap  = 0;       /* allocated capacity */

    /* Initialise with the first 5 terms on the first call */
    if (sz == 0) {
        cap = 16;                /* initial capacity – arbitrary */
        tbl = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!tbl) abort();       /* allocation failure */
        tbl[0] = (seq_t)1;
        tbl[1] = (seq_t)3;
        tbl[2] = (seq_t)8;
        tbl[3] = (seq_t)18;
        tbl[4] = (seq_t)38;
        sz   = 5;
    }

    /* Return if already computed */
    if (n < sz)
        return tbl[n];

    /* Grow the table if needed */
    if (n >= cap) {
        idx_t new_cap = cap;
        while (new_cap <= n)
            new_cap <<= 1;            /* double until large enough */
        seq_t *tmp = (seq_t *)realloc(tbl, new_cap * sizeof(seq_t));
        if (!tmp) abort();            /* allocation failure */
        tbl = tmp;
        cap = new_cap;
    }

    /* Compute missing terms up to n */
    for (idx_t i = sz; i <= n; ++i) {
        /* recurrence: a(i) = 3*a(i-1) - 2*a(i-2) - a(i-4) + a(i-5) */
        tbl[i] = 3 * tbl[i-1] - 2 * tbl[i-2] - tbl[i-4] + tbl[i-5];
    }
    sz = n + 1;                      /* update stored length */

    return tbl[n];
}
