/*
 * A057360  a(n) = floor(3*n/8).
 * https://oeis.org/A057360
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(3*n/8)
 * Range:     u8 n <= 681, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Compute floor(3*n/8) without overflow.
       Use division by 8 followed by remainder handling.      */
    seq_t q = (seq_t)n >> 3;          /* n / 8                */
    seq_t r = (seq_t)n & 7;           /* n % 8                */
    return q * 3 + (r * 3) / 8;       /* floor((n/8)*3 + (n%8)*3/8) */
}
