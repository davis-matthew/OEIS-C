/*
 * A132112  a(n) = n*(n+1)*(11*n+1)/6.
 * https://oeis.org/A132112
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+1)*(11*n+1)/6
 * Range:     u8 n <= 2, u16 n <= 17, u32 n <= 730, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t prod = nn * (nn + 1);          /* n*(n+1) */
    prod *= (11 * nn + 1);               /* * (11*n+1) */
    return prod / 6;                     /* divide by 6 */
}
