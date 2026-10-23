/*
 * A134163  a(n) = 1 + 12*n + 81*n^3 + n*(105*n + 81*n^3)/2.
 * https://oeis.org/A134163
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (3(3n + 1)^4 + 6(3n + 1)^3 - (3n + 1)^2 - 4(3n + 1) +
 *            2)/6
 * Range:     u8 n <= 0, u16 n <= 3, u32 n <= 99, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    seq_t t   = (seq_t)3 * sn + 1;
    seq_t t2  = t * t;
    seq_t t3  = t2 * t;
    seq_t t4  = t3 * t;
    seq_t num = (seq_t)3 * t4 + (seq_t)6 * t3 - t2 - (seq_t)4 * t + 2;
    return num / 6;
}
