/*
 * A173435  Inverse binomial transform of A143025, assuming offset zero there.
 * https://oeis.org/A173435
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = -4*a(n-1) - 6*a(n-2) - 4*a(n-3); Initial values: a(0)=8,
 *            a(1)=-6, a(2)=12, a(3)=-25
 * Range:     i8 n <= 5, i16 n <= 13, i32 n <= 29, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of already computed values */
    static seq_t *buf = NULL;
    static idx_t len = 0;          /* number of stored elements (buf[0]..buf[len-1]) */

    /* grow cache if necessary */
    if (n + 1 > len) {
        idx_t new_len = n + 1;
        seq_t *tmp = (seq_t*)realloc(buf, new_len * sizeof(seq_t));
        if (!tmp) {
            /* allocation failed – leave in undefined state but avoid crash */
            return 0;
        }
        buf = tmp;

        /* compute missing terms, starting from the first missing index */
        for (idx_t i = len; i <= n; ++i) {
            if (i == 0) {
                buf[0] = (seq_t)8;
            } else if (i == 1) {
                buf[1] = (seq_t)-6;
            } else if (i == 2) {
                buf[2] = (seq_t)12;
            } else if (i == 3) {
                buf[3] = (seq_t)-25;
            } else {
                /* recurrence: a(n) = -4*a(n-1) - 6*a(n-2) - 4*a(n-3) */
                buf[i] = -((seq_t)4 * buf[i-1])
                       - ((seq_t)6 * buf[i-2])
                       - ((seq_t)4 * buf[i-3]);
            }
        }
        len = new_len;
    }

    return buf[n];
}
