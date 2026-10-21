/*
 * A070551  a(n) = n^4 mod 23.
 * https://oeis.org/A070551
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^4 mod 23
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Reduce n modulo 23 first to keep arithmetic small */
    unsigned int m = (unsigned int)(n % 23U);
    unsigned int t = (m * m) % 23U;  /* m^2 mod 23 */
    t = (t * t) % 23U;               /* (m^2)^2 = m^4 mod 23 */
    return (seq_t)t;
}
