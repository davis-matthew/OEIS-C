/*
 * A229795  Number of 2 X 2 0..n arrays with rows and columns in
 *          lexicographically nondecreasing order.
 * https://oeis.org/A229795
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2*n^4 + 7*n^3 + 13*n^2 + 14*n + 6)/6
 * Range:     u8 n <= 2, u16 n <= 12, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t N  = (seq_t)n;
    seq_t n2 = N * N;
    seq_t n3 = n2 * N;
    seq_t n4 = n3 * N;
    seq_t num = (seq_t)2 * n4 + (seq_t)7 * n3 + (seq_t)13 * n2 + (seq_t)14 * N + (seq_t)6;
    return num / (seq_t)6;
}
