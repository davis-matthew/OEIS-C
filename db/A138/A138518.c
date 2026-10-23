/*
 * A138518  Expansion of (phi(-q) / phi(-q^5))^2 in powers of q where phi() is
 *          a Ramanujan theta function.
 * https://oeis.org/A138518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 20, i16 n <= 60, i32 n <= 210, i64 n <= 755,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A138518 (Expansion of (phi(-q)/phi(-q^5))^2).
    The generating function can be written as
        A(q) = ∏_{k≥1} (1 - q^k)^(-b_k)
    where the exponents b_k are periodic with period 10:
        b_k = [-4, -2, -4, -2, 0, -2, -4, -2, -4, 0]  (k starts at 1).
    This implementation builds the series up to the requested index
    using only integer arithmetic, so it works for all the supported
    seq_t types (signed integers or arbitrary‑precision classes).   */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Periodic exponent pattern (b_k) */
    static const int periodic[10] = { -4, -2, -4, -2, 0, -2, -4, -2, -4, 0 };

    /* Allocate array for the series up to degree n */
    seq_t *coeffs = (seq_t *)calloc((size_t)n + 1, sizeof(seq_t));
    if (!coeffs) return 0;                      /* allocation failure – return 0 */

    coeffs[0] = 1;                              /* A(0) = 1 */

    /* Process each factor (1 - q^k)^{-b_k} with e = -b_k ≥ 0 */
    for (idx_t k = 1; k <= n; ++k)
    {
        int bk = periodic[(int)((k - 1) % 10)];
        if (bk == 0) continue;                  /* factor = 1 */

        int e = -bk;                            /* e = 0,2,4 */

        /* Coefficients of (1 - x^k)^e :
           coeff[0] = 1, coeff[m] = C(e,m)*(-1)^m   for 1≤m≤e  */
        seq_t cof[5] = {0};
        cof[0] = 1;
        for (int m = 1; m <= e; ++m)
        {
            /* C(e,m) = C(e,m-1)*(e-m+1)/m */
            cof[m] = cof[m - 1] *
                     (seq_t)(e - m + 1) /
                     (seq_t)m;
            cof[m] = -cof[m];                   /* multiply by (-1)^m */
        }

        /* Convolve current series with this factor.
           Iterate indices decreasingly to avoid using updated values. */
        for (idx_t i = n; ; --i)
        {
            seq_t ai = coeffs[i];
            if (ai)
            {
                for (int m = 1; m <= e; ++m)
                {
                    idx_t idx = i + (idx_t)m * k;
                    if (idx > n) break;
                    coeffs[idx] += ai * cof[m];
                }
            }
            if (i == 0) break;
        }
    }

    seq_t result = coeffs[n];
    free(coeffs);
    return result;
}
