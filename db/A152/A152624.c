/*
 * A152624  Decimal expansion of 7/2.
 * https://oeis.org/A152624
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, cons
 * Formula:   not stated
 * Range:     u8 n <= 99, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(1) = 3, a(2) = 5, a(n) = 0 for n >= 3 */
seq_t a(idx_t n)
{
    if (n == 1) return 3;
    if (n == 2) return 5;
    return 0;
}
