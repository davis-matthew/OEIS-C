/*
 * A167028  Number of terms in the expansion of the determinant of a
 *          skew-symmetric matrix of order n.
 * https://oeis.org/A167028
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n is odd, otherwise a(2m) satisfies a(2m) =
 *            ((2m-1)*a(2(m-1))*2m + S_m) / (4m), where S_m = Σ_{k=0}^{m-1}
 *            (2m)!/(2k)! * a(2k), and S_{m+1} = (2m+2)(2m+1) * (S_m + a(2m)).
 *            This recurrence is derived from the exponential generating
 *            function (1-x^2)^(-1/4) exp(x^2/4)
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 13, u64 n <= 21, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n & 1u)                /* odd n */
        return 0;

    idx_t m = n / 2;            /* n = 2m */
    if (m == 0)                 /* n = 0 (not used) */
        return 0;
    if (m == 1)                 /* a(2) = 1 */
        return 1;

    /* b_prev holds a(2(m-1)), start with a(0) = 1 */
    seq_t b_prev = 1;                 /* a(0) */

    /* S_prev is S_m for the current value of m (initially S_1). */
    seq_t S_prev = (seq_t)2 * b_prev; /* S_1 = 2 * a(0) */

    for (idx_t cur_m = 2; cur_m <= m; ++cur_m)
    {
        /* Compute S_cur for this m using S_{m-1} and a(2(m-1)). */
        seq_t C = (seq_t)(2 * (cur_m - 1) + 2) * (seq_t)(2 * (cur_m - 1) + 1);
        /* C = (2m)(2m-1) */
        seq_t S_cur = C * (S_prev + b_prev);

        /* Compute a(2cur_m). */
        seq_t numerator   = (seq_t)(2 * cur_m - 1) * b_prev * (seq_t)(2 * cur_m) + S_cur;
        seq_t denominator = (seq_t)(4 * cur_m);
        seq_t b_cur       = numerator / denominator;

        if (cur_m == m)          /* reached the desired m */
            return b_cur;

        /* Prepare for next iteration. */
        S_prev = S_cur;
        b_prev = b_cur;
    }
    return 0;                     /* should never reach here */
}
