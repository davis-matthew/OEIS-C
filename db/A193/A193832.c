/*
 * A193832  Irregular triangle read by rows in which row n lists 2n-1 copies
 *          of 2n-1 and n copies of 2n, for n >= 1.
 * https://oeis.org/A193832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = the integer k such that * sum_{i=1}^{k-1} (i if i is odd
 *            else i/2) < n <= * sum_{i=1}^{k} (i if i is odd else i/2) * i.e.
 *            the sequence of successive integers where odd numbers appear * i
 *            times, even numbers appear i/2 times
 * Range:     u8 n <= 9926, u16 n <= 9926, u32 n <= 9926, u64 n <= 9926,
 *            u128 n <= 9926, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n < 1) return 0;           /* undefined for n < 1, return 0 */
    idx_t cum = 0;
    for (idx_t i = 1; ; ++i)
    {
        idx_t add = (i & 1) ? i : (i >> 1);
        cum += add;
        if (cum >= n)
            return (seq_t)i;
    }
}
