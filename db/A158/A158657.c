/*
 * A158657  a(n) = 784*n^2 - 28.
 * https://oeis.org/A158657
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 784*n^2 - 28
 * Range:     u8 none, u16 n <= 9, u32 n <= 2340, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* cast the constants to seq_t to avoid integer promotion pitfalls */
    const seq_t C = (seq_t)784;
    const seq_t M = (seq_t)28;
    seq_t nn = (seq_t)n * (seq_t)n;          /* n^2 in seq_t */
    return C * nn - M;
}
