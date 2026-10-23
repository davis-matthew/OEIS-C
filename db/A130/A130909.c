/*
 * A130909  Period 16: repeat [0, 1, 2, ..., 15]; a(n) = n mod 16.
 * https://oeis.org/A130909
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n mod 16
 * Range:     u8 n <= 95, u16 n <= 95, u32 n <= 95, u64 n <= 95, u128 n <= 95,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute n modulo 16, casting the result to seq_t. */
    return (seq_t)(n % 16);
}
