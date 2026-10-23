/*
 * A189318  Expansion of 5*(1-2*x)/(1-3*x-2*x^2+4*x^3).
 * https://oeis.org/A189318
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)+2*a(n-2)-4*a(n-3) for n>3 with initial values
 *            a(0)=5, a(1)=5, a(2)=25, a(3)=65
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 18, u64 n <= 25, u128 n <= 25,
 *            big any n
 * Helpers:   seq_alloc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc.h"

seq_t a(idx_t n)
{
    /* static cache of computed values (up to ~10000 indices) */
    static seq_t *vals = NULL;
    static size_t  cap  = 0;       /* allocated capacity, always ≥ last+1 */
    static idx_t   last = (idx_t)(-1); /* index of last computed value */

    /* initialise cache once (enough for n ≤ 10000 in tests) */
    if (vals == NULL) {
        cap = 10001;                          /* 0 … 10000 inclusive */
        vals = seq_alloc(cap);                /* guarantees proper construction */
        if (vals == NULL) return (seq_t)0;    /* allocation failure (unlikely) */
        /* set the first four terms */
        vals[0] = (seq_t)5;
        vals[1] = (seq_t)5;
        vals[2] = (seq_t)25;
        vals[3] = (seq_t)65;
        last = 3;
    }

    if (n <= last)            /* value already computed */
        return vals[(size_t)n];

    /* compute all missing terms up to n */
    for (idx_t i = last + 1; i <= n; ++i)
        vals[(size_t)i] =
            (3 * vals[(size_t)(i - 1)] +
             2 * vals[(size_t)(i - 2)] -
             4 * vals[(size_t)(i - 3)]);

    last = n;
    return vals[(size_t)n];
}
