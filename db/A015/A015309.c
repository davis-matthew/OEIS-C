/*
 * A015309  Gaussian binomial coefficient [ n,5 ] for q = -5.
 * https://oeis.org/A015309
 *
 * Offset:    5    (a(5) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = Gaussian binomial coefficient [n,5] evaluated at q = -5
 * Range:     i8 none, i16 n <= 6, i32 n <= 7, i64 n <= 10, i128 n <= 15,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 5) return 0;

    seq_t cur[6];
    seq_t prev[6];
    for (idx_t k = 0; k <= 5; ++k)
        cur[k] = (k == 0) ? 1 : 0;

    const seq_t q = -5;
    seq_t pow_q = 1;   /* q^0 */

    for (idx_t i = 1; i <= n; ++i) {
        pow_q *= q;                 /* q^i */

        /* copy current row to previous row */
        for (idx_t k = 0; k <= 5; ++k)
            prev[k] = cur[k];

        cur[0] = 1;
        seq_t factor = pow_q / q;   /* q^{i-1} */

        for (idx_t k = 1; k <= 5; ++k) {
            if (k > i) {
                cur[k] = 0;
            } else {
                seq_t addend = factor * prev[k-1];
                cur[k] = prev[k] + addend;
            }
            if (k < 5)
                factor /= q;        /* divide by q to get q^{i-k-1} for next k */
        }
    }

    return cur[5];
}
