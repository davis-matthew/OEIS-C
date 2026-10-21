/*
 * A214411  The maximum exponent k of 7 such that 7^k divides n.
 * https://oeis.org/A214411
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the largest exponent k such that 7^k divides n (7-adic
 *            valuation)
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Guard against the unlikely case that n is 0 */
    if (n == 0) return 0;

    seq_t count = 0;
    while ((n % 7) == 0)
    {
        n /= 7;
        ++count;
    }
    return count;
}
