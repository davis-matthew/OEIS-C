/*
 * A144138  Chebyshev polynomial of the second kind U(3,n).
 * https://oeis.org/A144138
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n*(2*n^2 - 1)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 812, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) {
        return 0;
    }
    seq_t nn = (seq_t)n;
    seq_t sq = nn * nn;
    seq_t t  = 2 * sq - 1;
    return 4 * nn * t;
}
