/*
 * A033093  Number of 0's when n is written in base b for 2<=b<=n+1.
 * https://oeis.org/A033093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ∑_{b=2}^{n} (number of 0‑digits in the representation of
 *            n in base b) (base b = n+1 contributes 0 zeros, so it is
 *            omitted)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;
    seq_t total = 0;
    for (idx_t b = 2; b <= n; ++b)
    {
        idx_t tmp = n;
        while (tmp > 0)
        {
            if ((tmp % b) == 0)
                ++total;
            tmp /= b;
        }
    }
    return total;
}
