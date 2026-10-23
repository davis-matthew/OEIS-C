/*
 * A179404  Number of ways to place 3 nonattacking kings on an n X n toroidal
 *          board.
 * https://oeis.org/A179404
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 for n < 4; a(n) = n^2*(n^4 - 27*n^2 + 194)/6 for n >= 4
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 43, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 4)
        return (seq_t)0;

    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;

    seq_t term = n4 - (seq_t)27 * n2 + (seq_t)194;
    seq_t num  = n2 * term;

    return num / (seq_t)6;
}
