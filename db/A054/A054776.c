/*
 * A054776  a(n) = 3*n*(3*n-1)*(3*n-2).
 * https://oeis.org/A054776
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = 3*n*(3*n-1)*(3*n-2)
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 542, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    seq_t t = (seq_t)n * 3;
    return t * (t - 1) * (t - 2);
}
