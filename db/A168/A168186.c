/*
 * A168186  Positive numbers that are not multiples of 12.
 * https://oeis.org/A168186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + ⌊(n-1)/11⌋
 * Range:     u8 n <= 234, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) gives the nth positive integer that is not a multiple of 12 */

seq_t a(idx_t n)
{
    /* n >= 1 (by definition of the sequence) */
    idx_t skip = (n - 1) / 11;     /* number of multiples of 12 skipped before a(n) */
    return (seq_t)n + (seq_t)skip;
}
