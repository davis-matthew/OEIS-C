/*
 * A096472  Numbers containing squares of Pythagorean triples in their divisor
 *          set.
 * https://oeis.org/A096472
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*60^2
 * Range:     u8 none, u16 n <= 18, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n times 3600, using seq_t for precise arithmetic. */
    return ((seq_t)n) * 3600;
}
