/*
 * A228656  Number of nX4 binary arrays with top left value 1 and no two ones
 *          adjacent horizontally, diagonally or antidiagonally.
 * https://oeis.org/A228656
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1) = 3, a(2) = 14, a(3) = 78,
   a(n) = 6*a(n-1) - 2*a(n-2) - 8*a(n-3) for n ≥ 4 */
#include <stdlib.h>

/* External linkage function computing the OEIS sequence A228656 */
seq_t a(idx_t n)
{
    /* Cache for memoisation: indices 1…max_comp */
    static seq_t *cache = NULL;     /* array indexed from 1, cache[0] unused */
    static size_t  cap    = 0;      /* allocated capacity (inclusive of index 0) */
    static size_t  max_comp = 0;    /* largest n that has already been computed */

    if (n < 1)          /* out of offset range; behaviour unspecified */
        return (seq_t)0;

    /* Ensure cache large enough for indices up to n */
    if (cap < (size_t)(n + 1)) {
        size_t newcap = (cap == 0) ? 4 : cap;
        while (newcap < (size_t)(n + 1))
            newcap <<= 1;                    /* double until sufficient */
        seq_t *tmp = (seq_t*)realloc(cache, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;           /* allocation failed */
        cache = tmp;
        cap   = newcap;
    }

    /* Initialise base values once */
    if (max_comp == 0) {
        cache[1] = (seq_t)3;
        cache[2] = (seq_t)14;
        cache[3] = (seq_t)78;
        max_comp = 3;
    }

    /* Return pre‑computed value if already available */
    if (n <= max_comp)
        return cache[n];

    /* Compute missing terms using the recurrence up to n */
    for (idx_t k = (idx_t)(max_comp + 1); k <= n; ++k) {
        seq_t term = cache[k-1] * (seq_t)6;
        term -= cache[k-2] * (seq_t)2;
        term -= cache[k-3] * (seq_t)8;
        cache[k] = term;
    }

    max_comp = n;
    return cache[n];
}
