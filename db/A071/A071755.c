/*
 * A071755  Expansion of (1+x^4*C^4)*C^2, where C = (1-(1-4*x)^(1/2))/(2*x) is
 *          g.f. for Catalan numbers, A000108.
 * https://oeis.org/A071755
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = coeff of C(x)^2 + x^4*C(x)^6. Let f(0)=1 and for k≥0:
 *            f(k+1) = f(k) * (2k+4)*(2k+3) / ((k+3)*(k+2)); (f(k) is the
 *            coefficient of C(x)^2 at index k). Let g(0)=1 and for m≥0:
 *            g(m+1) = g(m) * (2m+8)*(2m+7)*(m+3) / ((m+1)*(m+7)*(m+4)); (g(m)
 *            is the coefficient of C(x)^6 at index m). Then a(0)=...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 14, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

/* seq_t and idx_t are defined by the build environment. */

seq_t a(idx_t n)
{
    /* compute part coming from C(x)^2 : f(n) */
    seq_t f_prev = 1;  /* f[0] */
    for (idx_t k = 0; k < n; ++k) {
        seq_t num = f_prev;
        num *= (seq_t)(2*k + 4);
        num *= (seq_t)(2*k + 3);
        num /= (seq_t)(k + 3);
        num /= (seq_t)(k + 2);
        f_prev = num;
    }

    /* start with f(n) */
    seq_t result = f_prev;

    /* add part coming from x^4 * C(x)^6 : g(n-4) if n >= 4 */
    if (n >= 4) {
        idx_t m = n - 4;
        seq_t g_prev = 1;  /* g[0] */
        for (idx_t k = 0; k < m; ++k) {
            seq_t num = g_prev;
            num *= (seq_t)(2*k + 8);
            num *= (seq_t)(2*k + 7);
            num *= (seq_t)(k + 3);
            num /= (seq_t)(k + 1);
            num /= (seq_t)(k + 7);
            num /= (seq_t)(k + 4);
            g_prev = num;
        }
        result += g_prev;
    }

    return result;
}
