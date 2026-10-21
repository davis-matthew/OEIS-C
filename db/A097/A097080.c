/*
 * A097080  a(n) = 2*n^2 - 2*n + 3.
 * https://oeis.org/A097080
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2*n^2 - 2*n + 3
 * Range:     u8 n <= 11, u16 n <= 181, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;          /* convert index to the arithmetic type */
    return 2*nn*nn - 2*nn + 3;   /* exact integer arithmetic, no overflow for values that fit in seq_t */
}
