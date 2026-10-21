/*
 * A085736  Numbers n such that all groups of order n are solvable.
 * https://oeis.org/A085736
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor((n-1)/59)
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* All positive integers not divisible by 60. */
seq_t a(idx_t n)
{
    /* (n-1)/59 is integer division truncating toward zero */
    return (seq_t)n + (seq_t)((n - 1) / 59);
}
