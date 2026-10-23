/*
 * A144139  Chebyshev polynomial of the second kind U(4,n).
 * https://oeis.org/A144139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16*n^4-12*n^2+1
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 128, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t n2 = nn * nn;
    seq_t n4 = n2 * n2;
    return (seq_t)16 * n4 - (seq_t)12 * n2 + (seq_t)1;
}
