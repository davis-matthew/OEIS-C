/*
 * A179403  Number of ways to place 2 nonattacking kings on an n X n toroidal
 *          board.
 * https://oeis.org/A179403
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/2 * n^2 * (n-3) * (n+3)
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 287, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4) return 0;
    seq_t nn = (seq_t)n;
    seq_t res = nn * nn;
    res *= (nn - 3);
    res *= (nn + 3);
    res /= 2;
    return res;
}
