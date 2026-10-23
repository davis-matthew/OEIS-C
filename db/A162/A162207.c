/*
 * A162207  Number of reduced words of length n in the Weyl group D_4.
 * https://oeis.org/A162207
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the coefficient of xⁿ in (1‑x²)(1‑x⁴)²(1‑x⁶)/(1‑x)⁴. The
 *            numerator expands to 1‑x²‑2x⁴+x⁶+2x⁸+x¹⁰‑2x¹²‑x¹⁴+x¹⁶, and the
 *            denominator (1‑x)⁴ gives the binomial kernel C(k+3,3) for k≥0.
 *            Hence a(n)=∑_{m=0}^{16} coeffs[m]·C(n‑m+3,3) with coeffs
 *            ={1,0,‑1,0,‑2,0,1,0,2,0,1,0,‑2,0,‑1,0,1}
 * Range:     u8 n <= 101, u16 n <= 101, u32 n <= 101, u64 n <= 101,
 *            u128 n <= 101, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* coefficients of the numerator polynomial, indexed by exponent */
    static const int coeffs[17] = {
        1,   /* x⁰  */
        0,   /* x¹  */
       -1,   /* x²  */
        0,   /* x³  */
       -2,   /* x⁴  */
        0,   /* x⁵  */
        1,   /* x⁶  */
        0,   /* x⁷  */
        2,   /* x⁸  */
        0,   /* x⁹  */
        1,   /* x¹⁰ */
        0,   /* x¹¹ */
       -2,   /* x¹² */
        0,   /* x¹³ */
       -1,   /* x¹⁴ */
        0,   /* x¹⁵ */
        1    /* x¹⁶ */
    };

    /* The series is finite up to n=12; for larger n all terms vanish. */
    if (n > 12) {
        return (seq_t)0;
    }

    int64_t result = 0;

    for (idx_t m = 0; m <= 16; ++m) {
        int c = coeffs[m];
        if (c == 0) continue;          /* skip zero coefficients    */
        if (n < m) continue;           /* contribution zero for m>n */
        idx_t k = n - m;                /* non‑negative             */
        /* binomial (k+3 choose 3) = (k+3)(k+2)(k+1)/6 */
        int64_t bin = ((int64_t)(k + 3) * (k + 2) * (k + 1)) / 6;
        result += (int64_t)c * bin;
    }

    return (seq_t)result;
}
