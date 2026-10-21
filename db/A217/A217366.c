/*
 * A217366  a(n) = ((n+6) / gcd(n+6,4)) * (n / gcd(n,4)).
 * https://oeis.org/A217366
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n^2 + 6*n)/8 for even n, a(n) = n^2 + 6*n for odd n
 * Range:     u8 n <= 14, u16 n <= 254, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    if ((n & 1) == 0) {          /* even n */
        return (N * N + 6 * N) / 8;
    } else {                     /* odd n */
        return N * N + 6 * N;
    }
}
