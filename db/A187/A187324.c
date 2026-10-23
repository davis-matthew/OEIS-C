/*
 * A187324  a(n) = floor(n/2) + floor(n/3) - floor(n/4).
 * https://oeis.org/A187324
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n/2) + floor(n/3) - floor(n/4)
 * Range:     u8 n <= 436, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t half = sn / 2;
    seq_t third = sn / 3;
    seq_t fourth = sn / 4;
    return half + third - fourth;
}
