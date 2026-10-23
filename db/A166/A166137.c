/*
 * A166137  a(n) = 5*n*(n+1)/2 - 4.
 * https://oeis.org/A166137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 5*n*(n+1)/2 - 4 (OEIS A166137)
 * Range:     u8 n <= 9, u16 n <= 161, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* offset 1 – n>=1 is the valid domain */
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1);       /* n*(n+1) – always even */
    prod /= 2;                        /* divide by 2 exactly */
    prod *= 5;                        /* multiply by 5 */
    prod -= 4;                        /* subtract 4 */
    return prod;
}
