/*
 * A014556  Euler's "Lucky" numbers: n such that m^2-m+n is prime for
 *          m=0..n-1.
 * https://oeis.org/A014556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, nice
 * Formula:   a(n) = (A003173(n+3)+1)/4
 * Range:     u8 n <= 6, u16 n <= 6, u32 n <= 6, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n)
{
    /* only the first 6 terms are defined; for other indices return 0 */
    if (n == 0) return 0;
    if (n > 6) return 0;

    /* Heegner numbers:   1, 2, 3, 7, 11, 19, 43, 67, 163 */
    static const seq_t h[9] = {1, 2, 3, 7, 11, 19, 43, 67, 163};

    /* n runs from 1; we need h[n+2] to access the (n+3)-rd Heegner number */
    return (h[n + 2] + (seq_t)1) / (seq_t)4;
}
