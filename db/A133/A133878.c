/*
 * A133878  n modulo 8 repeated 8 times.
 * https://oeis.org/A133878
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1 + floor(n/8)) mod 8
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t q = (seq_t)n / 8;   /* floor(n/8) as seq_t */
    seq_t r = (q + 1) % 8;    /* (1 + floor(n/8)) mod 8 */
    return r;
}
