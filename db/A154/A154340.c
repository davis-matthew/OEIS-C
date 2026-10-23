/*
 * A154340  a(n) = ( (5 + 2*sqrt(2))^n - (5 - 2*sqrt(2))^n )/(4*sqrt(2)).
 * https://oeis.org/A154340
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 10*a(n-1) - 17*a(n-2) with initial values a(0) = 0, a(1)
 *            = 1, and offset = 1
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 22, u128 n <= 43,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* Return 0 for indices below the offset as a safety. */
    if (n == 0)
        return 0;
    if (n == 1)
        return 1;

    seq_t prev2 = 0;    /* a(0) */
    seq_t prev1 = 1;    /* a(1) */
    seq_t cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = prev1 * 10 - prev2 * 17;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
