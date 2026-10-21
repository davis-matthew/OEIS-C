/*
 * A212980  Number of (w,x,y) with all terms in {0,...,n} and w<x+y and x<y.
 * https://oeis.org/A212980
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10*(n-1)^3 + 15*(n-1)^2 + 2*(n-1) - 3 + 3*(n mod 2)) /
 *            24 (offset 1: a(1) = 0, a(2) = 1, …)
 * Range:     u8 n <= 3, u16 n <= 19, u32 n <= 41, u64 n <= 41, u128 n <= 41,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                       /* out of range, not used by tests */

    /* use m = n-1 so that the formula matches the offset 1 sequence */
    const seq_t m = (seq_t)(n - 1);
    const seq_t m2 = m * m;
    const seq_t m3 = m2 * m;

    /* numerator of the polynomial */
    seq_t num = (seq_t)10 * m3 + (seq_t)15 * m2 + (seq_t)2 * m - (seq_t)3;

    /* add 3 if n is odd (since (n-1)+1 = n) */
    if ( (n & 1ULL) != 0 )
        num += (seq_t)3;

    /* divide by 24 */
    return num / (seq_t)24;
}
