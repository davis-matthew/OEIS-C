/*
 * A092820  a(0) = 1; for n>0, a(n) = 16 times sum of cubes of divisors of n.
 * https://oeis.org/A092820
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 16 * sum_{d|n} d^3 (a(0) = 1)
 * Range:     u8 n <= 2, u16 n <= 15, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 1;
    seq_t sum = 0;
    for (idx_t d = 1; d * d <= n; ++d)
    {
        if (n % d == 0)
        {
            seq_t d_cube = (seq_t)d * d * d;
            sum += d_cube;
            idx_t e = n / d;
            if (e != d)
            {
                seq_t e_cube = (seq_t)e * e * e;
                sum += e_cube;
            }
        }
    }
    return sum * 16;
}
