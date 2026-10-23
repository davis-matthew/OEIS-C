/*
 * A159333  Roman factorial of n.
 * https://oeis.org/A159333
 *
 * Offset:    -2    (a(-2) is the first term)
 * Keywords:  easy, sign, less
 * Formula:   a(n) = n! for n >= 0 a(n) = ((-1)^(n+1)) / (-n-1)! for n < 0
 *            (offset -2)
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* The factorials are cached incrementally. */
    static seq_t *fac = NULL;          /* memory for factorials 0! .. m!   */
    static idx_t cap = 0;              /* allocated size            */
    static idx_t len = 0;              /* next free slot (first unused) */

    /* Initialise the cache on first use: 0! = 1. */
    if (cap == 0) {
        cap = 1;
        fac = (seq_t *)malloc((size_t)cap * sizeof(seq_t));
        if (!fac) abort();
        fac[0] = (seq_t)1;
        len = 1;
    }

    /* Determine the factorial index to evaluate.   */
    idx_t m;
    int sign = 1;
    if (n >= 0) {
        m = n;
    } else {
        /* n < 0:  (-n - 1)!  and a sign factor  */
        m = -n - 1;
        sign = ((n + 1) & 1) ? -1 : 1;  /* (-1)^(n+1)  */
    }

    /* Grow the cache up to m if necessary. */
    if (m >= len) {
        if (m >= cap) {
            /* Double the capacity until we can hold m.            */
            while (cap <= m) cap <<= 1;
            seq_t *tmp = (seq_t *)realloc(fac, (size_t)cap * sizeof(seq_t));
            if (!tmp) abort();
            fac = tmp;
        }
        /* Compute missing factorials. */
        for (idx_t i = len; i <= m; ++i) {
            fac[(size_t)i] = fac[(size_t)(i - 1)] * (seq_t)i;
        }
        len = m + 1;
    }

    /* Return the appropriately signed value. */
    return sign * fac[(size_t)m];
}
