/*
 * A147790  a(1) = 3, a(n) = round(a(n-1)*3/2) for n > 1, using round-to-even
 *          method.
 * https://oeis.org/A147790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 24, u32 n <= 39, u64 n <= 39, u128 n <= 39,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=3; a(n)=round(a(n-1)*3/2) for n>1 (banker‑rounding) */
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Cache of previously computed terms (1‑based indexing).  */
    static seq_t *cache      = NULL;   /* Array of values.              */
    static size_t  cap      = 0;       /* Allocated capacity (elements).*/
    static idx_t   size     = 0;       /* Highest index stored.         */

    if (n == 0) return (seq_t)0;       /* Undefined for n==0.           */

    /* Initialise cache on first call.  */
    if (size == 0) {
        cap = 2;                       /* Need indices 0..1 (ignore 0). */
        cache = (seq_t *)malloc(cap * sizeof(seq_t));
        if (!cache) return (seq_t)0;   /* Allocation failed.            */
        cache[1] = (seq_t)3;           /* a(1)=3.                       */
        size = 1;
    }

    /* Expand cache if required.  */
    if ((size_t)n + 1 > cap) {
        size_t new_cap = cap * 2;
        if (new_cap <= (size_t)n + 1) new_cap = (size_t)n + 1;
        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
        if (!tmp) return (seq_t)0;     /* Allocation failed.            */
        cache = tmp;
        cap   = new_cap;
    }

    /* Compute new terms if n is beyond what we have.  */
    for (idx_t i = size + 1; i <= n; ++i) {
        seq_t prev = cache[(size_t)(i - 1)];
        seq_t prod = prev * (seq_t)3;              /* a(n‑1)*3           */
        seq_t q    = prod >> 1;                    /* floor(prod/2)      */
        seq_t rem  = prod & (seq_t)1;              /* prod mod 2         */
        if (rem != 0) {                /* Fractional part is 0.5 */
            if ((q & (seq_t)1) != 0)   /* q is odd → round up   */
                q += (seq_t)1;         /* q even → round down   */
        }
        cache[(size_t)i] = q;
    }
    size = n;
    return cache[(size_t)n];
}
