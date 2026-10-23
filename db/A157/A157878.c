/*
 * A157878  Expansion of x*(1+x)/(x^2-30*x+1).
 * https://oeis.org/A157878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 1, a(2) = 31, a(n) = 30*a(n-1)-a(n-2) (n>2)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 7, u64 n <= 14, u128 n <= 27,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* offset not used */
    if (n == 1) return 1;
    if (n == 2) return 31;
    seq_t prev2 = 1;                 /* a(1) */
    seq_t prev1 = 31;                /* a(2) */
    seq_t cur;
    for (idx_t i = 3; i <= n; ++i) {
        cur = 30 * prev1 - prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
