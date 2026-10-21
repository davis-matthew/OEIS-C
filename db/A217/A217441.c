/*
 * A217441  Numbers k such that 26*k+1 is a square.
 * https://oeis.org/A217441
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(13*n - 2)/2 if n is even, * a(n) = (n-1)*(13*n - 11)/2
 *            if n is odd
 * Range:     u8 n <= 7, u16 n <= 101, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Cast n to seq_t once so arithmetic stays in the correct type. */
    seq_t N = (seq_t)n;
    if ((n & 1) == 0) {
        /* n is even: a(n) = n*(13*n - 2)/2 */
        return N * (13 * N - 2) / 2;
    } else {
        /* n is odd: a(n) = (n-1)*(13*n - 11)/2 */
        return (N - 1) * (13 * N - 11) / 2;
    }
}
