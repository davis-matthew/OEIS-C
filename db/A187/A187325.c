/*
 * A187325  a(n) = floor(n/2) + floor(n/3) + floor(n/4) + floor(n/5).
 * https://oeis.org/A187325
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n/2) + floor(n/3) + floor(n/4) + floor(n/5)
 * Range:     u8 n <= 199, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t t1 = sn / 2;
    seq_t t2 = sn / 3;
    seq_t t3 = sn / 4;
    seq_t t4 = sn / 5;
    return t1 + t2 + t3 + t4;
}
