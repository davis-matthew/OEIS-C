/*
 * A173127  a(n) = sinh((2n-1)*arcsinh(3)).
 * https://oeis.org/A173127
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = 38*a(n-1) - a(n-2), a(0) = -3, a(1) = 3
 * Range:     i8 n <= 2, i16 n <= 3, i32 n <= 6, i64 n <= 12, i128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0u) return (seq_t)(-3);
    if (n == 1u) return (seq_t)( 3);
    seq_t a_prev2 = (seq_t)(-3);   /* a(0) */
    seq_t a_prev1 = (seq_t)( 3);   /* a(1) */
    seq_t a_curr;

    for (idx_t i = 2u; i <= n; ++i)
    {
        a_curr = (seq_t)(38) * a_prev1 - a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = a_curr;
    }
    return a_prev1;
}
