/*
 * A089708  a(1) = 1, a(2) = 2, a(n) = a(n-1) + d where d is the sum of the
 *          absolute differences between all pairs of previous terms.
 * https://oeis.org/A089708
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)*(a(n-1)-a(n-2)) + a(n-3), a(1)=1, a(2)=2, a(3)=3,
 *            a(4)=7
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    static seq_t *buf = NULL;     /* cache of computed terms, 0‑based */
    static idx_t buf_len = 0;     /* number of terms stored (a(1) .. a(buf_len)) */

    if (n < 1)
        return (seq_t)0;          /* undefined for non‑positive indices */

    if (buf_len < n) {
        idx_t old_len = buf_len;
        buf_len = n;
        buf = (seq_t *)realloc(buf, (size_t)buf_len * sizeof(seq_t));
        if (!buf)
            abort();              /* out of memory */

        /* initialise the first few terms if not already done */
        if (old_len == 0) {
            buf[0] = (seq_t)1;    /* a(1) */
            old_len = 1;
        }
        if (old_len == 1 && buf_len > 1) {
            buf[1] = (seq_t)2;    /* a(2) */
            old_len = 2;
        }
        if (old_len == 2 && buf_len > 2) {
            buf[2] = (seq_t)3;    /* a(3) */
            old_len = 3;
        }
        if (old_len == 3 && buf_len > 3) {
            buf[3] = (seq_t)7;    /* a(4) */
            old_len = 4;
        }

        /* compute remaining terms using the recurrence */
        for (idx_t i = old_len; i < buf_len; ++i) {
            idx_t k = i + 1;            /* term number (1‑based) */
            if (k >= 5) {
                seq_t diff = buf[i-1] - buf[i-2];      /* a(k-1) - a(k-2) */
                seq_t factor = (seq_t)(k + 1);          /* (k+1) */
                buf[i] = factor * diff + buf[i-3];     /* recurrence */
            }
        }
    }

    return buf[n-1];
}
