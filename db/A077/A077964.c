/*
 * A077964  Expansion of 1/(1+2*x^2-2*x^3).
 * https://oeis.org/A077964
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(0) = 1; a(1) = 0; a(2) = -2; for n >= 3: a(n) = -2*a(n-2) +
 *            2*a(n-3) (from 1/(1+2x^2-2x^3))
 * Range:     i8 n <= 11, i16 n <= 22, i32 n <= 46, i64 n <= 91, i128 n <= 185,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;      /* memoized values */
    static idx_t  bufsize = 0;     /* allocated size */

    if (n >= bufsize) {            /* need more space */
        idx_t oldsize = bufsize;
        idx_t newsize = n + 1;     /* exactly enough for n */

        seq_t *newbuf = (seq_t *)malloc(newsize * sizeof(seq_t));
        if (!newbuf) {
            /* if allocation fails, abort (should not happen for n ≈ 10000) */
            exit(1);
        }

        /* copy old values */
        if (oldsize > 0)
            memcpy(newbuf, buf, oldsize * sizeof(seq_t));

        /* initialise new entries to 0 (they will be overwritten) */
        if (newsize > oldsize)
            memset((char *)newbuf + oldsize * sizeof(seq_t), 0,
                   (newsize - oldsize) * sizeof(seq_t));

        free(buf);
        buf = newbuf;
        bufsize = newsize;

        /* compute missing terms from oldsize up to n */
        for (idx_t i = oldsize; i <= n; ++i) {
            if (i == 0)          buf[i] = 1;
            else if (i == 1)     buf[i] = 0;
            else if (i == 2)     buf[i] = -2;
            else                 buf[i] = -2 * buf[i - 2] + 2 * buf[i - 3];
        }
    } else {
        /* enough space, but some later terms may be uninitialized
           (possible if n is smaller than previously cached max). */
        for (idx_t i = 3; i <= n; ++i) {
            if (buf[i] == 0)   /* we cannot test for zero reliably, so recompute all */
                buf[i] = -2 * buf[i - 2] + 2 * buf[i - 3];
        }
    }

    return buf[n];
}
