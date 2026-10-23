/*
 * A122112  a(n) = 4*a(n-2) - a(n-1), with a(0)=1, a(1)=-2.
 * https://oeis.org/A122112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 4*a(n-2) - a(n-1), with a(0)=1, a(1)=-2
 * Range:     i8 n <= 5, i16 n <= 11, i32 n <= 22, i64 n <= 46, i128 n <= 93,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* External linkage as required */
seq_t a(idx_t n)
{
    /* static cache of computed values */
    static seq_t *buf = NULL;
    static idx_t size = 0;          /* number of terms stored (buf[0..size-1]) */

    /* initialise cache for the first two terms */
    if (!buf) {
        buf = (seq_t *)malloc(2 * sizeof(seq_t));
        if (!buf) return 0;              /* allocation failure -> zero */
        buf[0] = (seq_t)1;
        buf[1] = (seq_t)-2;
        size = 2;
    }

    /* fast path for small indices */
    if (n == 0) return buf[0];
    if (n == 1) return buf[1];

    /* grow cache if necessary */
    if (n >= size) {
        seq_t *tmp = (seq_t *)realloc(buf, (n + 1) * sizeof(seq_t));
        if (!tmp) return 0;              /* allocation failure -> zero */
        buf = tmp;
        /* compute new terms using the recurrence */
        for (idx_t i = size; i <= n; ++i) {
            buf[i] = (seq_t)4 * buf[i - 2] - buf[i - 1];
        }
        size = n + 1;
    }

    return buf[n];
}
