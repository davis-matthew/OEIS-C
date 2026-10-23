/*
 * A143465  A McMullen transform involving x->x+1/x of Lehmer's polynomial
 *          gives the polynomial used to get this expansion sequence: p(x)=1 +
 *          x + 10 x^2 + 8 x^3 + 44 x^4 + 28 x^5 + 113 x^6 + 57 x^7 + 191 x^8
 *          + 79 x^9 + 227 x^10 + 79 x^11 + 191 x^12 + 57 x^13 + 113 x^14 + 28
 *          x^15 + 44 x^16 + 8 x^17 + 10 x^18...
 * https://oeis.org/A143465
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, sign
 * Formula:   a(n) = -∑_{k=1}^{20} p_k · a(n‑k), a(1)=1 with p_k the
 *            coefficients of the palindromic polynomial p(x)=1 + 1x
 *            +10x^2+8x^3+44x^4+28x^5+113x^6+57x^7+191x^8+79x^9
 *            +227x^10+79x^11+191x^12+57x^13+113x^14+28x^15+44x^16
 *            +8x^17+10x^18+1x^19+1x^20
 * Range:     i8 n <= 6, i16 n <= 18, i32 n <= 31, i64 n <= 31, i128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static const int p_coeffs[21] = {
    0,  /* unused; p_coeffs[k] stores coefficient for x^k, 1≤k≤20 */
    1, 10, 8, 44, 28, 113, 57, 191, 79, 227,
    79, 191, 57, 113, 28, 44, 8, 10, 1, 1
};

seq_t a(idx_t n)
{
    /* Memoization buffer: indices 1..buf_len are valid */
    static seq_t *buf = NULL;
    static idx_t buf_len = 0;

    if (n < 1)
        return 0;   /* offset is 1 */

    if (n > buf_len) {
        idx_t old_len = buf_len;
        idx_t new_len = n + 1;          /* include index 0 (unused) */

        seq_t *new_buf = (seq_t *)malloc(new_len * sizeof(seq_t));
        if (!new_buf)
            exit(EXIT_FAILURE);

        /* copy existing values */
        for (idx_t i = 1; i <= old_len; ++i)
            new_buf[i] = buf[i];

        /* free old buffer */
        if (buf)
            free(buf);

        buf = new_buf;
        buf_len = n;

        /* compute new entries */
        for (idx_t i = old_len + 1; i <= n; ++i) {
            if (i == 1) {
                buf[i] = 1;   /* a(1) = 1 */
                continue;
            }
            seq_t sum = 0;
            /* a(i) = -∑_{k=1}^{20} p_k · a(i-k)  (only for k<i) */
            for (idx_t k = 1; k <= 20; ++k) {
                if (i > k) {
                    sum += buf[i - k] * (seq_t)p_coeffs[k];
                }
            }
            buf[i] = -sum;
        }
    }

    return buf[n];
}
