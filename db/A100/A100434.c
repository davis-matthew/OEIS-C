/*
 * A100434  Expansion of g.f. (1+x)*(3+x)/(1+6*x^2+x^4).
 * https://oeis.org/A100434
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 3, a(1) = 4 a(n) = -2*a(n-1) - 3*a(n-2) if n is even a(n)
 *            = (4*a(n-1) - a(n-2))/3 if n is odd
 * Range:     i8 n <= 4, i16 n <= 11, i32 n <= 22, i64 n <= 48, i128 n <= 98,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static storage for memoization */
    static seq_t *buf      = NULL;
    static idx_t buf_cap   = 0;  /* capacity (number of elements allocated) */
    static idx_t buf_len   = 0;  /* number of computed elements (next free index) */

    /* ensure that buf can hold up to n */
    if (buf_len <= n) {
        idx_t needed = n + 1;          /* we need indices 0 .. n inclusive */
        if (buf_cap < needed) {
            idx_t new_cap = needed * 2;  /* grow capacity to avoid frequent reallocs */
            seq_t *tmp = (seq_t*)realloc(buf, new_cap * sizeof(seq_t));
            buf = tmp;
            buf_cap = new_cap;
        }

        /* initialise base values if they have not been set yet */
        if (buf_len == 0) {
            buf[0] = (seq_t)3;
            buf_len = 1;
        }
        if (buf_len == 1) {
            buf[1] = (seq_t)4;
            buf_len = 2;
        }

        /* compute missing terms up to n */
        for (idx_t i = buf_len; i <= n; ++i) {
            if ((i & 1) == 0) { /* even i */
                buf[i] = -(seq_t)2 * buf[i-1] - (seq_t)3 * buf[i-2];
            } else {           /* odd i */
                buf[i] = ((seq_t)4 * buf[i-1] - buf[i-2]) / (seq_t)3;
            }
        }
        buf_len = n + 1;
    }

    return buf[n];
}
