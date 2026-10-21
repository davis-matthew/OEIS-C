/*
 * A073762  a(n) = 24*n - 12.
 * https://oeis.org/A073762
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24*n - 12
 * Range:     u8 n <= 11, u16 n <= 2731, u32 n <= 3000, u64 n <= 3000,
 *            u128 n <= 3000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Convert n to seq_t to avoid overflow in intermediate multiplication */
    return (seq_t)n * 24 - 12;
}
