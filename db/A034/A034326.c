/*
 * A034326  Hours struck by a clock.
 * https://oeis.org/A034326
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1) mod 12 + 1
 * Range:     u8 n <= 81, u16 n <= 81, u32 n <= 81, u64 n <= 81, u128 n <= 81,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 by the sequence definition. */
    /* Compute the value using only integer arithmetic. */
    idx_t mod = (n - 1) % 12;
    return (seq_t)(mod + 1);
}
