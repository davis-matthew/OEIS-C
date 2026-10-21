/*
 * A235988  Sum of the partition parts of 3n into 3 parts.
 * https://oeis.org/A235988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*n^3 - 3*n*floor(n^2/4)
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t ns   = (seq_t)n;          /* Convert index to seq_t */
    seq_t n2   = ns * ns;           /* n^2 */
    seq_t term1 = 3 * ns * n2;      /* 3*n^3 */
    seq_t term2 = 3 * ns * (n2 / 4);/* 3*n*floor(n^2/4) */
    return term1 - term2;
}
