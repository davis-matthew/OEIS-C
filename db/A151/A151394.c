/*
 * A151394  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0,0), ending on the vertical axis and consisting of n steps taken
 *          from {(-1, -1), (0, -1), (0, 1), (1, -1)}.
 * https://oeis.org/A151394
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk
 * Formula:   a(n) = Σ_{d=0}^{⌊n/2⌋} binom(n,d) * (n-2d+1)/(n-d+1) * M_d where
 *            M_d are Motzkin numbers. Motzkin numbers satisfy: M_0 = 1, M_1 =
 *            1, (n+2)·M_n = (2n+1)·M_{n-1} + 3(n-1)·M_{n-2} (n ≥ 2)
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 23, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t d_max = n / 2;              /* maximum number of down steps   */
    seq_t result = 0;                 /* accumulated answer             */
    seq_t C = 1;                      /* binomial(n, d)  (current d)    */

    /* previous two Motzkin numbers: M_{d-2} and M_{d-1} */
    seq_t M_n_minus_2 = 1;             /* M_0  */
    seq_t M_n_minus_1 = 1;             /* M_1  */
    seq_t M_n;                        /* M_d for current iteration     */

    for (idx_t d = 0; d <= d_max; ++d)
    {
        /* current Motzkin number */
        if (d == 0)
            M_n = M_n_minus_2;        /* M_0 = 1 */
        else if (d == 1)
            M_n = M_n_minus_1;        /* M_1 = 1 */
        else
        {
            seq_t rhs = (seq_t)(2*d + 1) * M_n_minus_1 +
                        (seq_t)(3*(d-1)) * M_n_minus_2;
            M_n = rhs / (d + 2);      /* exact division */
        }

        /* binomial(n,d) * (n-2d+1)/(n-d+1)  */
        seq_t numerator   = (seq_t)(n - 2*d + 1);
        seq_t denominator = (seq_t)(n - d + 1);
        seq_t Nd = C * numerator;
        Nd /= denominator;

        result += Nd * M_n;           /* add contribution */

        /* update binomial for next d */
        if (d < d_max)
        {
            C = C * (seq_t)(n - d);
            C /= (d + 1);
        }

        /* prepare Motzkin numbers for next iteration */
        if (d >= 2)
        {
            M_n_minus_2 = M_n_minus_1;
            M_n_minus_1 = M_n;
        }
    }
    return result;
}
