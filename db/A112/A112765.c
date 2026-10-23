/*
 * A112765  Exponent of highest power of 5 dividing n. Or, 5-adic valuation of
 *          n.
 * https://oeis.org/A112765
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = exponent of highest power of 5 dividing n (5-adic
 *            valuation)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t cnt = 0;
    while (n % 5 == 0)
    {
        cnt++;
        n /= 5;
    }
    return cnt;
}
