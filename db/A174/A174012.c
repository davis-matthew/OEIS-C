/*
 * A174012  a(n) = 3 * A064680(n).
 * https://oeis.org/A174012
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3*n)/2 for even n; 6*n for odd n
 * Range:     u8 n <= 42, u16 n <= 64, u32 n <= 64, u64 n <= 64, u128 n <= 64,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if ((n & 1) == 0) {
        return (N / 2) * (seq_t)3;
    } else {
        return N * (seq_t)6;
    }
}
