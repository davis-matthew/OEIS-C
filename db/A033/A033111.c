/*
 * A033111  Number of 9's when n is written in base b for 2<=b<=n+1.
 * https://oeis.org/A033111
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = sum_{b=2}^{n+1} #{digits equal to 9 in base‑b
 *            representation of n}
 * Range:     u8 n <= 90, u16 n <= 90, u32 n <= 90, u64 n <= 90, u128 n <= 90,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;

    /* Iterate over all bases b from 2 to n+1 */
    for (idx_t b = 2; b <= n + 1; ++b)
    {
        idx_t temp = n;
        while (temp)
        {
            idx_t digit = temp % b;
            if (digit == 9)
                sum += 1;
            temp /= b;
        }
    }
    return sum;
}
