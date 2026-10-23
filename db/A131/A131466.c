/*
 * A131466  a(n) = 5n^4 - 4n^3 + 3n^2 - 2n + 1.
 * https://oeis.org/A131466
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n^4 - 4*n^3 + 3*n^2 - 2*n + 1
 * Range:     u8 n <= 2, u16 n <= 10, u32 n <= 171, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N = (seq_t)n;
    seq_t t = 5;
    t = t * N - 4;
    t = t * N + 3;
    t = t * N - 2;
    t = t * N + 1;
    return t;
}
