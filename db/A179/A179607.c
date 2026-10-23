/*
 * A179607  Eight white kings and one red king on a 3 X 3 chessboard. G.f.: (1
 *          + 2*x - 4*x^2)/(1 - 2*x - 8*x^2).
 * https://oeis.org/A179607
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 2*a(n-1) + 8*a(n-2), a(0) = 1, a(1) = 4, a(2) = 12
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 16, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Base cases */
    if (n == 0)
        return (seq_t)1;
    if (n == 1)
        return (seq_t)4;
    if (n == 2)
        return (seq_t)12;

    /* a(i-1) and a(i-2) for the current step */
    seq_t prev1 = (seq_t)12;  /* a(2) */
    seq_t prev2 = (seq_t)4;   /* a(1) */
    seq_t cur   = 0;

    for (idx_t i = 3; i <= n; ++i)
    {
        cur  = (seq_t)2 * prev1 + (seq_t)8 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return cur;
}
