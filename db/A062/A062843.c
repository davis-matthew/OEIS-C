/*
 * A062843  Maximum number of ones in the representation of n in any base.
 * https://oeis.org/A062843
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = max_{b≥2} number of 1 digits in the base‑b representation
 *            of n
 * Range:     u8 n <= 105, u16 n <= 105, u32 n <= 105, u64 n <= 105,
 *            u128 n <= 105, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;         /* a(1) = 1 */

    seq_t maxOnes = 0;
    for (idx_t base = 2; base <= n; ++base)
    {
        idx_t temp = n;
        seq_t count = 0;
        while (temp > 0)
        {
            if (temp % base == (idx_t)1) ++count;
            temp /= base;
        }
        if (count > maxOnes) maxOnes = count;
    }
    return maxOnes;
}
