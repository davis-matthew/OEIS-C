/*
 * A047673  Row 4 of square array defined in A047671.
 * https://oeis.org/A047673
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 - 2*n^3 + 5*n^2 - 4*n + 1
 * Range:     u8 n <= 4, u16 n <= 16, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n3 = n2 * nn;
    seq_t n4 = n3 * nn;
    seq_t res = n4 - (seq_t)2 * n3 + (seq_t)5 * n2 - (seq_t)4 * nn + (seq_t)1;
    return res;
}
