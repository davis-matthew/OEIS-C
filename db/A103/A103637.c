/*
 * A103637  a(n) = Sum_{d|n, d==1 mod 3} d^2.
 * https://oeis.org/A103637
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{d|n, d == 1 (mod 3)} d^2
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d)
    {
        if (n % d == 0)
        {
            if ((d % 3) == 1)
            {
                seq_t sq = (seq_t)d * d;
                sum += sq;
            }
            idx_t e = n / d;
            if (e != d && (e % 3) == 1)
            {
                seq_t sq = (seq_t)e * e;
                sum += sq;
            }
        }
    }
    return sum;
}
