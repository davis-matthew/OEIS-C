/*
 * A188838  Number of n X 4 binary arrays without the pattern 0 1 diagonally
 *          or vertically.
 * https://oeis.org/A188838
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n^4 + 22*n^3 + 155*n^2 + 326*n - 144)/24 for n>1,
 *            a(1)=16
 * Range:     u8 n <= 1, u16 n <= 10, u32 n <= 200, u64 n <= 200,
 *            u128 n <= 200, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1)
        return (seq_t)16;
    seq_t nn  = (seq_t)n;
    seq_t n2  = nn * nn;
    seq_t n3  = n2 * nn;
    seq_t n4  = n3 * nn;
    seq_t numer = n4 + 22 * n3 + 155 * n2 + 326 * nn - 144;
    return numer / 24;
}
