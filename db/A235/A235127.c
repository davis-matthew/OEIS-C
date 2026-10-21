/*
 * A235127  Greatest k such that 4^k divides n.
 * https://oeis.org/A235127
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = valuation(n,4)
 * Range:     u8 n <= 9452, u16 n <= 9452, u32 n <= 9452, u64 n <= 9452,
 *            u128 n <= 9452, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;

    seq_t k = 0;
    idx_t t = n;

    /* Count how many times n is divisible by 4 = 2^2 */
    while ((t & 3) == 0)   /* last two bits are zero => divisible by 4 */
    {
        t >>= 2;           /* remove a factor of 4 */
        k++;
    }

    return k;
}
