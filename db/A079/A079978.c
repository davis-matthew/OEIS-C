/*
 * A079978  Characteristic function of multiples of three.
 * https://oeis.org/A079978
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 if n is a multiple of 3, otherwise 0
 * Range:     u8 n <= 86, u16 n <= 86, u32 n <= 86, u64 n <= 86, u128 n <= 86,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* A 3‑periodic sequence: 1,0,0,1,0,0,... */
    return (seq_t)((n % 3) == 0);
}
