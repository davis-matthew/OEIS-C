/*
 * A049863  a(n) = Sum{a(k): k=0,1,2,...,n-4,n-2,n-1}; a(n-3) is not a
 *          summand; initial terms are 0,3,4.
 * https://oeis.org/A049863
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*a(n-1) - a(n-3) + a(n-4), with a(0)=0, a(1)=3, a(2)=4,
 *            a(3)=7
 * Range:     u8 n <= 8, u16 n <= 17, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Handle the first three indices directly */
    if (n < 3) {
        /* static array to avoid reallocation in every call */
        static const seq_t init[3] = {0, 3, 4};
        return init[n];
    }

    /* Static cache for memoisation */
    static seq_t *buf   = NULL;   /* array of computed values */
    static idx_t  cap   = 0;      /* allocated capacity (>=4 when initialised) */
    static idx_t  maxc  = 0;      /* largest index already computed (>=3) */

    /* Allocate / grow buffer if needed */
    if (cap == 0) {
        /* First call: ensure capacity of at least 4 */
        cap = 4;
        buf = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!buf) abort();              /* Allocation failed */
        /* Initialise the first four terms */
        buf[0] = 0;
        buf[1] = 3;
        buf[2] = 4;
        buf[3] = 7;
        maxc = 3;
    } else if ((idx_t)(n + 1) > cap) {
        /* Grow buffer to at least n+1 */
        idx_t new_cap = cap * 2;
        if (new_cap < n + 1) new_cap = n + 1;
        seq_t *tmp = (seq_t *)realloc(buf, new_cap * sizeof(seq_t));
        if (!tmp) abort();              /* Allocation failed */
        buf = tmp;
        cap = new_cap;
    }

    /* Compute missing terms in ascending order */
    for (idx_t i = maxc + 1; i <= n; ++i) {
        /* Recurrence: a(i) = 2*a(i-1) - a(i-3) + a(i-4) */
        buf[i] = 2 * buf[i - 1] - buf[i - 3] + buf[i - 4];
    }
    maxc = n;
    return buf[n];
}
