/*
 * A008580  Crystal ball sequence for planar net 3.6.3.6.
 * https://oeis.org/A008580
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = (18*n^2+2*(-1)^n*n+18*n-7*(-1)^n-1)/8 for n>0, a(0) = 1
 * Range:     u8 n <= 3, u16 n <= 120, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;
    seq_t N = (seq_t)n;
    seq_t numerator;
    if ((n & 1) == 0) {               /* n even */
        numerator = 18 * N * N + 20 * N - 8;
    } else {                          /* n odd */
        numerator = 18 * N * N + 16 * N + 6;
    }
    return numerator / 8;
}
