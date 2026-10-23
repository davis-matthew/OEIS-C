/*
 * A147792  A quadrisection of A061042.
 * https://oeis.org/A147792
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = A061042(4n+4) The values satisfy: For k = n+1 and m = 4k,
 *            if k is even -> a(n) = m^2 if k is odd -> a(n) = m^2 / d, where
 *            d = 16 when k mod 8 is 1 or 7, d = 8 when k mod 8 is 3 or 5.
 *            This rule reproduces the first 24 terms and a(N) for all N ≥ 0
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 45, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* compute k = n + 1 */
    idx_t k = n + 1ULL;          /* unsigned addition is safe */
    /* m = 4 * k */
    seq_t m = ((seq_t)k) * 4;
    /* m^2 */
    seq_t m2 = m * m;

    if ((k & 1ULL) == 0ULL)      /* k even -> a = m^2 */
        return m2;

    /* k odd: choose divisor 8 or 16 depending on k mod 8 */
    idx_t r = k % 8ULL;
    if (r == 1ULL || r == 7ULL)
        return m2 / 16;          /* divide by 16 */
    else
        return m2 / 8;           /* divide by 8 */
}
