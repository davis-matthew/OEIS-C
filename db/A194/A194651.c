/*
 * A194651  Number of ways to place 3 nonattacking kings on an n X n
 *          cylindrical chessboard.
 * https://oeis.org/A194651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1/6*n*(n^5 - 27*n^3 + 18*n^2 + 194*n - 228) for n>=4;
 *            a(n)=0 for n<4
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 31, u64 n <= 31, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;           // n^2
    seq_t n3 = n2 * nn;           // n^3
    seq_t n5 = n3 * n2;           // n^5
    seq_t expr = n5
                 - (seq_t)27 * n3
                 + (seq_t)18 * n2
                 + (seq_t)194 * nn
                 - (seq_t)228;
    seq_t res = nn * expr / 6;
    return res;
}
