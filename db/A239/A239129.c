/*
 * A239129  a(n) = 18*n - 1, n >= 1, the second column of triangle A239127
 *          related to the Collatz problem.
 * https://oeis.org/A239129
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 18*n - 1
 * Range:     u8 n <= 14, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* The OEIS offset is 1, but the formula works for any n ≥ 1. */
    return (seq_t)18 * (seq_t)n - (seq_t)1;
}
