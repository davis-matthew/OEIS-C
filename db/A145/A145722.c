/*
 * A145722  Expansion of f(q) * f(q^5) / phi(-q^2)^2 in powers of q where f(),
 *          phi() are Ramanujan theta functions.
 * https://oeis.org/A145722
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 33, u32 n <= 100, u64 n <= 333,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A145722: Coefficients of
   G.f. = Product_{k ≥ 1} (1 + x^{2k})(1 - x^{10k})(1 + x^{5k}) / ((1 - x^k)(1 + x^{10k}))
   Computed via the Euler transform with period‑20 exponents 
   [1,2,1,1,2,2,1,1,1,0,1,1,1,2,2,1,1,2,1,0].
   Recurrence: 
      Let e_k be that periodic sequence. Start with coeff[0]=1.
      For k = 1 .. n
          if e_k = 1:   coeff[i] += coeff[i - k]   (i = k .. n)
          if e_k = 2:   coeff[i] += coeff[i - k]   twice (two successive convolutions)
   The result a(n) is coeff[n].
*/

#include <stdlib.h>

static const idx_t period20[20] = {
    1,2,1,1,2,2,1,1,1,0,
    1,1,1,2,2,1,1,2,1,0
};

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)1;

    /* Allocate coefficient array up to n */
    seq_t *coeff = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!coeff) return (seq_t)0;           /* allocation failure (not expected) */

    /* Initialize */
    for (idx_t i = 0; i <= n; ++i) coeff[i] = (seq_t)0;
    coeff[0] = (seq_t)1;

    /* Main DP */
    for (idx_t k = 1; k <= n; ++k) {
        idx_t e = period20[(k - 1) % 20];
        if (e == 0) continue;          /* skip, factor is 1 */

        if (e == 1) {                  /* multiply by 1/(1 - x^k) */
            for (idx_t i = k; i <= n; ++i) {
                coeff[i] += coeff[i - k];
            }
        } else if (e == 2) {           /* multiply twice by 1/(1 - x^k) */
            for (idx_t i = k; i <= n; ++i) {
                coeff[i] += coeff[i - k];
            }
            for (idx_t i = k; i <= n; ++i) {
                coeff[i] += coeff[i - k];
            }
        }
    }

    seq_t result = coeff[n];
    free(coeff);
    return result;
}
