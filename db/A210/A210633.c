/*
 * A210633  G.f. for Ehrhart quasi-polynomials for hyperplane arrangements of
 *          type E_7.
 * https://oeis.org/A210633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = coefficient of x^n in
 *            x^18/((1-x)^2*(1-x^2)^3*(1-x^3)^2*(1-x^4))
 * Range:     u8 n <= 26, u16 n <= 46, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The code precomputes the coefficients of the product of the four generating
   series and then evaluates the convolution with (1-x)^-2.',
   using a linear-time prefix‑sum trick. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n < 18) return 0;

    idx_t m = n - 18;                     /* index into the product sequence */
    static idx_t cache_len = 0;           /* number of values precomputed  */
    static seq_t *h12 = NULL;             /* coefficient array of f2*f3    */
    static seq_t *g   = NULL;             /* coefficient array of f2*f3*f4  */
    static seq_t *w   = NULL;             /* final convolution with f1     */

    idx_t newlen = m + 1;
    if (newlen > cache_len) {
        /* Allocate new arrays of required length */
        size_t sz = (size_t)newlen;
        if (h12) free(h12); h12 = (seq_t*)calloc(sz, sizeof(seq_t));
        if (g  ) free(g ); g  = (seq_t*)calloc(sz, sizeof(seq_t));
        if (w  ) free(w ); w  = (seq_t*)calloc(sz, sizeof(seq_t));

        /* Step 1: compute h12 = (1/(1-x^2)^3) * (1/(1-x^3)^2) */
        for (idx_t a = 0; a <= newlen/2; ++a) {
            /* choose(a+2, 2) = (a+2)(a+1)/2 */
            seq_t bin = ((seq_t)(a + 2) * (a + 1)) / 2;
            idx_t base = 2 * a;
            for (idx_t b = 0; base + 3*b < newlen; ++b) {
                idx_t idx = base + 3*b;
                seq_t term = bin * ((seq_t)(b + 1));
                h12[idx] += term;
            }
        }

        /* Step 2: compute g = h12 * (1/(1-x^4))  ->  g[n] = h12[n] + g[n-4] */
        for (idx_t i = 0; i < newlen; ++i) {
            g[i] = h12[i];
            if (i >= 4) g[i] += g[i-4];
        }

        /* Step 3: compute w = (1/(1-x)^2) * g
           Using w[n] = (n+1)*Gsum[n] - Ssum[n] where
           Gsum[n] = Σ_{k=0..n} g[k]
           Ssum[n] = Σ_{k=0..n} k*g[k] */
        seq_t Gsum = 0, Ssum = 0;
        for (idx_t i = 0; i < newlen; ++i) {
            Gsum += g[i];
            Ssum += (seq_t)i * g[i];
            w[i] = ((seq_t)(i + 1) * Gsum) - Ssum;
        }

        cache_len = newlen;
    }

    return w[m];
}
