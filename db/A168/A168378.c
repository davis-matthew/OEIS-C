/*
 * A168378  a(n) = 3 + 8*floor(n/2).
 * https://oeis.org/A168378
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3 + 8*floor(n/2)
 * Range:     u8 n <= 63, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* Undefined for n < 1, arbitrary value */
    seq_t k = (seq_t)(n >> 1);      /* floor(n/2) */
    return 3 + (k * 8);
}
