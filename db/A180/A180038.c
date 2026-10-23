/*
 * A180038  Eight white queens and one red queen on a 3 X 3 chessboard. G.f.:
 *          (1 - 3*x)/(1 - 5*x - 2*x^2).
 * https://oeis.org/A180038
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 5*a(n-1) + 2*a(n-2) with a(0) = 1, a(1) = 2
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 13, u64 n <= 26, u128 n <= 53,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    if (n == 1) return 2;

    seq_t a_prev2 = 1; /* a(n-2) */
    seq_t a_prev1 = 2; /* a(n-1) */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t cur = 5 * a_prev1 + 2 * a_prev2;
        a_prev2 = a_prev1;
        a_prev1 = cur;
    }
    return a_prev1;
}
