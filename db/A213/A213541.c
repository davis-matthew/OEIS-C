/*
 * A213541  a(n) = n AND n^2, where AND is the bitwise AND operator.
 * https://oeis.org/A213541
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, less, look
 * Formula:   a(n) = n AND n^2
 * Range:     u8 n <= 271, u16 n <= 8192, u32 n <= 8192, u64 n <= 8192,
 *            u128 n <= 8192, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    return sn & (sn * sn);
}
