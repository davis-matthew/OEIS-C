/*
 * A052666  E.g.f. 1/(1-x-3x^2).
 * https://oeis.org/A052666
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(0) = 1, a(1) = 1 a(n+2) = (3*n^2 + 9*n + 6) * a(n) + (n+2) *
 *            a(n+1) (This is equivalent to the recurrence given in the OEIS
 *            entry.)
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 9, u64 n <= 16, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)
        return (seq_t)1;

    seq_t a_prev2 = (seq_t)1;   /* a(0) */
    seq_t a_prev1 = (seq_t)1;   /* a(1) */
    seq_t a_curr = (seq_t)0;    /* a(n+2) will be computed */

    /* Loop advancing n from 0 to (n-2) to compute a(n) */
    for (idx_t i = 0; i <= n - 2; ++i)
    {
        /* coeff = 3*i*i + 9*i + 6  (fits in seq_t for all i <= n) */
        seq_t coeff = (seq_t)3 * i * i + (seq_t)9 * i + (seq_t)6;
        seq_t term1 = coeff * a_prev2;
        seq_t term2 = (i + (seq_t)2) * a_prev1;

        a_curr = term1 + term2;

        /* Shift for next iteration */
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }

    return a_curr;
}
