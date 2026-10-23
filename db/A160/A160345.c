/*
 * A160345  Numerator of Hermite(n, 27/31).
 * https://oeis.org/A160345
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, frac
 * Formula:   a(n) = numerator of H_n(27/31) * H_0 = 1, H_1 = 2x -> x = 27/31
 *            gives 54/31 * Recurrence for physicists' Hermite polynomials: *
 *            H_{0} = 1 * H_{1} = 2x * H_{n} = 2x H_{n-1} - 2(n-1) H_{n-2} (n
 *            ≥ 2) * Substituting x = 27/31 and writing H_n = P_n / 31^n
 *            yields * P_0 = 1 * P_1 = 54 * P_n = 54 P_{n-1}...
 * Range:     i8 n <= 1, i16 n <= 2, i32 n <= 5, i64 n <= 9, i128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)54;

    seq_t prev2 = (seq_t)1;   /* P_{n-2} */
    seq_t prev1 = (seq_t)54;  /* P_{n-1} */
    seq_t curr;               /* P_n  */

    for (idx_t i = 2; i <= n; ++i)
    {
        /* term1 = 54 * P_{n-1} */
        seq_t term1 = (seq_t)54 * prev1;

        /* term2 = 1922 * (n-1) * P_{n-2} */
        seq_t term2 = ((seq_t)(i - 1) * (seq_t)1922) * prev2;

        curr = term1 - term2;

        /* shift for next iteration */
        prev2 = prev1;
        prev1 = curr;
    }
    return prev1;
}
