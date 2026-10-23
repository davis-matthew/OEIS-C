/*
 * A160838  G.f.: (1+38*x+263*x^2+484*x^3+263*x^4+38*x^5+x^6)/(1-x)^7.
 * https://oeis.org/A160838
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 68*n^6/45 + 68*n^5/15 + 91*n^4/9 + 38*n^3/3 + 467*n^2/45
 *            + 24*n/5 + 1 * * The coefficients of the generating function
 *            (1+38*x+263*x^2+484*x^3+263*x^4+38*x^5+x^6)/(1-x)^7 * give a
 *            simple convolution with binomial coefficients: * a(n) =
 *            Σ_{k=0..6} c_k * binom(n-k+6,6), * with c = [1, 38, 263...
 * Range:     u8 n <= 1, u16 n <= 5, u32 n <= 37, u64 n <= 1516,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Small helper: binomial coefficient binom(m,6) computed multiplicatively.
 * For arbitrary‑precision seq_t this is exact; for small types it stays within range
 * for the values of n tested (up to the point where a(n) still fits). */
static inline seq_t binom_6(idx_t m)
{
    if (m < 6) return 0;
    seq_t r = 1;
    /* Compute C(m,6) = Π_{i=1}^{6} (m-6+i)/i   */
    for (idx_t i = 1; i <= 6; ++i) {
        r = (r * (m - 6 + i)) / i;
    }
    return r;
}

seq_t a(idx_t n)
{
    /* Coefficients of the generating polynomial */
    static const seq_t coeff[7] = {
        (seq_t)1,  (seq_t)38,  (seq_t)263,  (seq_t)484,
        (seq_t)263, (seq_t)38,   (seq_t)1
    };

    seq_t sum = 0;
    for (idx_t k = 0; k <= 6; ++k) {
        if (n < k) continue;               /* binom(n-k+6,6)=0 when n<k */
        idx_t m = n - k + 6;
        seq_t bin = binom_6(m);
        sum += coeff[k] * bin;
    }
    return sum;
}
