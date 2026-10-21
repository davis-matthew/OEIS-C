/*
 * A201722  Number of n X 1 0..4 arrays with rows and columns
 *          lexicographically nondecreasing and no element equal to the number
 *          of horizontal and vertical neighbors equal to itself.
 * https://oeis.org/A201722
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4 (n=1), 10 (n=2), 3*n^2 - 6*n + 11 (n>2)
 * Range:     u8 n <= 10, u16 n <= 148, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)4;
    if (n == 2) return (seq_t)10;
    seq_t N = (seq_t)n;
    return 3 * N * N - 6 * N + 11;
}
