/*
 * A129370  a(n) = n^2 - (n-1)^2*(1 - (-1)^n)/8.
 * https://oeis.org/A129370
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = n^2 - (n-1)^2*(1 - (-1)^n)/8
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t sq = sn * sn;

    /* even n: a(n) = n^2 */
    if ((n & 1) == 0)
        return sq;

    /* odd n: a(n) = n^2 - (n-1)^2/4 */
    seq_t m   = sn - 1;
    seq_t sqm = m * m;
    return sq - (sqm / 4);
}
