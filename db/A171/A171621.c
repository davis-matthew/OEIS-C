/*
 * A171621  Numerator of 1/4 - 1/n^2, each fourth term multiplied by 4.
 * https://oeis.org/A171621
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = -(-5+3*(-1)^n)*(-4+n^2)/8
 * Range:     u8 n <= 16, u16 n <= 256, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;
    seq_t sub = sq - (seq_t)4;
    if ((n & 1) == 0)
        return sub / (seq_t)4;   // even n: (n^2-4)/4
    else
        return sub;             // odd n:  n^2-4
}
