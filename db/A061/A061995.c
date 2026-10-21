/*
 * A061995  Number of ways to place 2 nonattacking kings on an n X n board.
 * https://oeis.org/A061995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - 1) * (n - 2) * (n^2 + 3*n - 2) / 2
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 303, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3) {
        return 0;
    }
    seq_t nn   = (seq_t)n;
    seq_t t1   = nn - 1;
    seq_t t2   = nn - 2;
    seq_t inner = nn * nn + (seq_t)3 * nn - 2;
    seq_t prod  = t1 * t2 * inner;
    return prod / 2;
}
