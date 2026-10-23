/*
 * A014066  Inverse of 57th cyclotomic polynomial.
 * https://oeis.org/A014066
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = coefficient of x^n in 1/Φ_57(x), periodic with period 57
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    /* Φ_57(x) is the 57th cyclotomic polynomial of degree 36.
       Its coefficients are computed once and stored in f[0..36].
       The inverse series a(x) = 1/Φ_57(x) is then generated up to
       degree 56 (one full period).  The resulting sequence is
       periodic with period 57: a(n) = a[n mod 57].              */
    static seq_t f[37];          /* coefficients of Φ_57(x)          */
    static seq_t a_coef[57];     /* first 57 terms of the inverse seq*/
    static bool init = false;    /* initialization flag              */

    if (!init) {
        /* Compute Φ_57(x) = Φ_3(x^19)/Φ_3(x) using (x^2+x+1) * Q(x)
           = x^38 + x^19 + 1.  The coefficients of Q(x)=Σ_k f[k]x^k
           satisfy   f[k] + f[k-1] + f[k-2] = b[k]   with
           b[0]=b[19]=b[38]=1, all other b[k] = 0.          */
        seq_t num[39] = {0};
        num[0] = 1;
        num[19] = 1;
        num[38] = 1;

        seq_t prev2 = 0;  /* f[k-2]  */
        seq_t prev1 = 0;  /* f[k-1]  */
        for (int k = 0; k <= 38; ++k) {
            seq_t bk = num[k];
            seq_t ak = bk - prev1 - prev2;   /* f[k]          */
            if (k <= 36) f[k] = ak;           /* store needed terms */
            prev2 = prev1;
            prev1 = ak;
        }

        /* Generate the inverse series a(x) = 1/Φ_57(x) up to degree 56.
           Recurrence:  a[0] = 1  and
                         a[n] = – Σ_{k=1}^{min(n,36)} f[k] * a[n‑k]          */
        a_coef[0] = 1;
        for (int i = 1; i < 57; ++i) {
            seq_t sum = 0;
            int maxk = (i < 36) ? i : 36;
            for (int k = 1; k <= maxk; ++k) {
                sum += f[k] * a_coef[i - k];
            }
            a_coef[i] = -sum;
        }
        init = true;
    }

    return a_coef[(size_t)(n % 57)];
}
