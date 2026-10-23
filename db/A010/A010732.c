/*
 * A010732  a(n) = (17-(-1)^n)/2.
 * https://oeis.org/A010732
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (17 - (-1)^n)/2
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* use parity of n: even → 8, odd → 9 */
    return ((n & 1) == 0) ? (seq_t)8 : (seq_t)9;
}
