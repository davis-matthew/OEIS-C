/*
 * A133877  N modulo 7 repeated 7 times.
 * https://oeis.org/A133877
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (1+floor(n/7)) mod 7
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    idx_t m = n / 7;
    idx_t m_mod = m % 7;
    return (seq_t)((1 + m_mod) % 7);
}
