/*
 * A086008  Number of 0's in decimal expansion of n^2.
 * https://oeis.org/A086008
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = number of 0's in decimal expansion of n^2
 * Range:     u8 n <= 18, u16 n <= 348, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* 0^2 = 0 has one zero */

    seq_t sq = (seq_t)n * (seq_t)n;
    seq_t cnt = 0;

    while (sq > 0) {
        if (sq % 10 == 0)
            cnt++;
        sq /= 10;
    }

    return cnt;
}
