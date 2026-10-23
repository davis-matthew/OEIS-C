/*
 * A141534  Derived from the centered polygonal numbers: start with the first
 *          triangular number, then the sum of the first square number and the
 *          second triangular number, then the sum of first pentagonal number,
 *          the second square number and the third triangular number, and so
 *          on and so on...
 * https://oeis.org/A141534
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-1)*(n^3+11*n^2-38*n+120)/24 (for n>1); a(1)=1
 * Range:     u8 n <= 2, u16 n <= 14, u32 n <= 563, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)1;
    seq_t m      = (seq_t)n - 1;                 /* m = n-1 */
    seq_t m2     = m * m;                        /* m^2 */
    seq_t m3     = m2 * m;                       /* m^3 */
    seq_t m4     = m2 * m2;                      /* m^4 */

    /* numerator = m^4 + 14*m^3 + 94*m - 13*m^2 */
    seq_t numerator = m4 + 14 * m3 + 94 * m;
    numerator -= 13 * m2;

    /* divide by 24 */
    return numerator / 24;
}
