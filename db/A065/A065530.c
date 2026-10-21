/*
 * A065530  If n is odd then a(n) = n, else a(n) = n*(n+2).
 * https://oeis.org/A065530
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n for odd n * a(n) = n*(n+2) for even n * The function
 *            returns the n-th term of OEIS A065530 with offset 0
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t m = (seq_t)n;
    return (n & 1) ? m : m * (m + 2);
}
