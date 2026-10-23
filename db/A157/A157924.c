/*
 * A157924  a(n) = 98*n - 1.
 * https://oeis.org/A157924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 98*n - 1
 * Range:     u8 n <= 2, u16 n <= 668, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Since the sequence is defined for n ≥ 1, the formula works directly. */
    return (seq_t)n * (seq_t)98 - (seq_t)1;
}
