/*
 * A160747  Expansion of (1 + 10*x + 20*x^2 + 10*x^3 + x^4)/(1-x)^5.
 * https://oeis.org/A160747
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 + 7*n*(n+1)*(n^2+n+2)/4
 * Range:     u8 n <= 2, u16 n <= 13, u32 n <= 222, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    seq_t sn = (seq_t)n;
    /* Compute n*(n+1)/2 safely (n*(n+1) is even) */
    seq_t a1 = sn * (sn + 1) / 2;
    /* Compute (n^2+n+2)/2 safely ((n^2+n+2) is even) */
    seq_t c1 = (sn * sn + sn + 2) / 2;
    /* Now product/4 = a1 * c1 */
    seq_t prod = a1 * c1;
    /* Multiply by 7 and add 1 */
    seq_t part = prod * 7;
    return part + 1;
}
