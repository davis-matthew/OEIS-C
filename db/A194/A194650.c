/*
 * A194650  Number of ways to place 2 nonattacking kings on an n X n
 *          cylindrical chessboard.
 * https://oeis.org/A194650
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n * (n^3 - 9*n + 6)) / 2 for n >= 3; a(1) = a(2) = 0
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 38, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 3)
        return 0;

    seq_t nn  = (seq_t)n;        // n
    seq_t n2  = nn * nn;         // n^2
    seq_t n3  = n2 * nn;         // n^3
    seq_t expr = n3 - 9 * nn + 6; // n^3 - 9*n + 6
    seq_t res  = nn * expr;      // n * expr
    res /= 2;                    // divide by 2

    return res;
}
