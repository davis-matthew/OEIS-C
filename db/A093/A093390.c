/*
 * A093390  a(n) = floor(n/9) + floor((n+1)/9) + floor((n+2)/9).
 * https://oeis.org/A093390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/9) + floor((n+1)/9) + floor((n+2)/9)
 * Range:     u8 n <= 770, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t one = (seq_t)1;
    seq_t two = (seq_t)2;
    return sn / 9 + (sn + one) / 9 + (sn + two) / 9;
}
