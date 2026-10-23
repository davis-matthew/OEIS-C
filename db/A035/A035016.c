/*
 * A035016  Fourier coefficients of E_{0,4}.
 * https://oeis.org/A035016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy, nice
 * Formula:   a(0)=1; for n>0, a(n)=16*sum_{d|n, 0<d} (-1)^d d^3
 * Range:     i8 n <= 2, i16 n <= 12, i32 n <= 491, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;      /* a(0) */
    seq_t sum = 0;
    idx_t d;
    for (d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            /* first divisor d */
            seq_t d1 = (seq_t)d;
            seq_t sign1 = (d & 1) ? -(seq_t)1 : (seq_t)1;
            seq_t d1sq = d1 * d1;
            seq_t d1cub = d1sq * d1;
            sum += sign1 * d1cub;

            /* second divisor n/d, if distinct */
            idx_t d2_ui = n / d;
            if (d2_ui != d)
            {
                seq_t d2 = (seq_t)d2_ui;
                seq_t sign2 = (d2_ui & 1) ? -(seq_t)1 : (seq_t)1;
                seq_t d2sq = d2 * d2;
                seq_t d2cub = d2sq * d2;
                sum += sign2 * d2cub;
            }
        }
    }
    return (seq_t)16 * sum;
}
