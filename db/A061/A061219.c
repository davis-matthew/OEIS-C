/*
 * A061219  a(n) is the largest number which can be formed with no zeros,
 *          using least number of digits and having digit sum = n.
 * https://oeis.org/A061219
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number consisting of floor(n/9) digits 9, followed by the
 *            digit n%9 if n%9>0, otherwise just floor(n/9) digits 9.
 *            Equivalently: k = n/9; r = n%9; if (r==0) a(n) = 10^k - 1; else
 *            a(n) = (10^k - 1)*10 + r; (The result has minimal digits and is
 *            the largest such number.)
 * Range:     u8 n <= 18, u16 n <= 36, u32 n <= 81, u64 n <= 171,
 *            u128 n <= 342, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)0;          /* undefined for offset 1, but safe */
    idx_t k = n / 9;             /* how many 9's */
    int r = (int)(n % 9);        /* remaining digit (0..8) */
    seq_t result = 0;
    for (idx_t i = 0; i < k; ++i)
        result = result * 10 + 9;   /* append a 9 */

    if (r != 0)
        result = result * 10 + r;   /* append the remaining digit */

    return result;
}
