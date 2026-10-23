/*
 * A049464  Number of n-photon quenched skeletons.
 * https://oeis.org/A049464
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0)=1. For n≥1 define g[0]=0, g[1]=1 and for n≥2 * g[n] =
 *            2*Σ_{k=1}^{n-1} k*g[k]*g[n-k] + * Σ_{i=1}^{n-1} g[i]*g[n-i] + *
 *            Σ_{i=1}^{n-2} g[i]*g[n-1-i] - 2*g[n-1] * (where g[n]=a(n) for
 *            n≥1). This recurrence follows from the * differential equation 0
 *            = 2*x*y*deriv(y,x) + (1+x)*y^2 - * (2*x+1)*y +...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 16, u128 n <= 28,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    /* Allocate an array of n+1 terms (we ignore g[0] when n==0). */
    seq_t *g = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!g)
        return (seq_t)0;                /* fallback – allocation failed */

    g[0] = (seq_t)0;
    g[1] = (seq_t)1;

    for (idx_t cur = 2; cur <= n; ++cur) {
        seq_t s1 = 0, s12 = 0, s13 = 0;

        /* S1 = 2 * Σ_{k=1}^{cur-1} k * g[k] * g[cur-k] */
        for (idx_t k = 1; k < cur; ++k) {
            seq_t term = (seq_t)k * g[k];
            term *= g[cur - k];
            s1 += term;
        }
        s1 *= 2;

        /* S12 = Σ_{i=1}^{cur-1} g[i] * g[cur-i] */
        for (idx_t i = 1; i < cur; ++i) {
            s12 += g[i] * g[cur - i];
        }

        /* S13 = Σ_{i=1}^{cur-2} g[i] * g[cur-1-i] */
        for (idx_t i = 1; i + 1 < cur; ++i) {
            s13 += g[i] * g[cur - 1 - i];
        }

        g[cur] = s1 + s12 + s13 - 2 * g[cur - 1];
    }

    seq_t result = g[n];
    free(g);
    return result;
}
