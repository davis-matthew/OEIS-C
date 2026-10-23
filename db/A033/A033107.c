/*
 * A033107  Number of 7's when n is written in base b for 2<=b<=n+1.
 * https://oeis.org/A033107
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = ∑_{b=2}^{n+1} (number of digit 7 in the base‑b
 *            representation of n)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sum = 0;
    if (n == 0) return sum;                 /* Offset starts at 1, but guard against 0 */

    /* Loop over all bases from 2 up to n+1 */
    for (idx_t base = 2; base <= n + 1; ++base)
    {
        idx_t tmp = n;
        while (tmp > 0)
        {
            if (tmp % base == 7) sum++;     /* Count each digit 7 */
            tmp /= base;
        }
    }
    return sum;
}
