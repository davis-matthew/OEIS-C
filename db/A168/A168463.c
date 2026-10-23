/*
 * A168463  a(n) = 5 + 11*floor(n/2).
 * https://oeis.org/A168463
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5 + 11*floor(n/2)
 * Range:     u8 n <= 45, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* floor division (n/2) is already an integer division */
    seq_t half = (seq_t)(n / 2);
    return (seq_t)5 + (seq_t)11 * half;
}
