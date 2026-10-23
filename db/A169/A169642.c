/*
 * A169642  a(n) = A005408(n) * A022998(n).
 * https://oeis.org/A169642
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4*n^2 + 2*n if n even (n>0) a(n) = 2*n^2 + n if n odd
 *            a(0) = 0
 * Range:     u8 n <= 7, u16 n <= 127, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if (n == 0) {
        return (seq_t)0;
    }
    if ((n & 1) == 0) {          // n is even
        return 4 * N * N + 2 * N;
    } else {                     // n is odd
        return 2 * N * N + N;
    }
}
