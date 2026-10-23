/*
 * A047883  Squares on unbounded chessboard for which the least number of
 *          knight's moves from corner (0,0) is n.
 * https://oeis.org/A047883
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0)=0, a(1)=2, a(2)=9, a(3)=20, a(4)=27, a(5)=32; for n >= 6:
 *            a(n) = a(n-1) + 7 (equiv. a(n) = 32 + 7*(n-5))
 * Range:     u8 n <= 34, u16 n <= 34, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    static const seq_t pre[6] = {0, 2, 9, 20, 27, 32};

    if (n <= 5)
        return pre[(idx_t)n];

    return pre[5] + (seq_t)(n - 5) * 7;
}
