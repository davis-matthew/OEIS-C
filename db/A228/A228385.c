/*
 * A228385  Number of n X 3 binary arrays with top left value 1 and no two
 *          ones adjacent horizontally or vertically.
 * https://oeis.org/A228385
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 17, u64 n <= 35, u128 n <= 69,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(n) = 2*a(n-1) + 6*a(n-2) - a(n-4)  (n >= 5)
   Initial values: a(1)=2, a(2)=5, a(3)=21, a(4)=72 */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of previously computed values */
    static seq_t *cached = NULL;
    static idx_t cap = 0;
    static idx_t sz  = 0;
    static const seq_t base[4] = { (seq_t)2, (seq_t)5, (seq_t)21, (seq_t)72 };

    /* For n <= 4 just return the precomputed base value */
    if (n <= 4)
        return base[n - 1];

    /* Initial allocation of the cache (first 4 values) */
    if (cached == NULL) {
        cap = (n > 4) ? n : 4;
        cached = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cached)
            exit(EXIT_FAILURE);                       /* allocation failure */
        cached[0] = base[0];
        cached[1] = base[1];
        cached[2] = base[2];
        cached[3] = base[3];
        sz = 4;
    }
    /* Grow cache if necessary */
    else if (n > cap) {
        idx_t newcap = cap;
        while (newcap < n)
            newcap <<= 1;                            /* double until big enough */
        seq_t *tmp = (seq_t *)realloc(cached, newcap * sizeof(seq_t));
        if (!tmp)
            exit(EXIT_FAILURE);                       /* reallocation failure */
        cached = tmp;
        cap = newcap;
    }

    /* Compute missing values up to n using the recurrence */
    if (sz < n) {
        for (idx_t i = sz + 1; i <= n; ++i) {
            /* a(i) = 2*a(i-1) + 6*a(i-2) - a(i-4) */
            seq_t val = (seq_t)(2 * cached[i - 2]          /* a(i-1)   */
                                 + 6 * cached[i - 3]     /* a(i-2)   */
                                 - cached[i - 5]);       /* a(i-4)   */
            cached[i - 1] = val;
        }
        sz = n;
    }

    return cached[n - 1];
}
