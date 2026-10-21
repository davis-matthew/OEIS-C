/*
 * A224334  Number of idempotent 3 X 3 0..n matrices of rank 2.
 * https://oeis.org/A224334
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*n^2 + 12*n + 3
 * Range:     u8 n <= 5, u16 n <= 103, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* For n < 1 the offset is not defined; return 0 safely. */
    if (n < 1) return (seq_t)0;
    seq_t N = (seq_t)n;
    return 6 * N * N + 12 * N + (seq_t)3;
}
