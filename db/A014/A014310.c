/*
 * A014310  Inverse of 301st cyclotomic polynomial.
 * https://oeis.org/A014310
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   a(n) = the coefficient of x^n in the formal power series
 *            expansion of 1 / Φ_301(x), where Φ_301 is the 301st cyclotomic
 *            polynomial. The sequence is periodic with period 301. a(0) = 1
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include types and functions provided by the build system. */
#include <stdbool.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Static tables are initialised only once. */
    static bool init = false;
    static seq_t vals[301];    /* a(0) … a(300)                */
    static seq_t phi[253];     /* coefficients of Φ_301(x)      */

    if (!init) {
        /* Coefficients of the cyclotomic polynomial Φ_301(x)
           are obtained by the rational expression
                (x^301‑1)(x‑1) / ((x^43‑1)(x^7‑1)).
           After long division we get a polynomial of degree 252. */

        /* Numerator: (x^302 – x^301 – x + 1) */
        seq_t num[303] = {0};
        num[302] =  1;  /* x^302 */
        num[301] = -1;  /* -x^301 */
        num[1]   = -1;  /* -x   */
        num[0]   =  1;  /* +1   */

        /* Denominator: (x^50 – x^43 – x^7 + 1) */
        seq_t den[51] = {0};
        den[50] =  1;  /* x^50 */
        den[43] = -1;  /* -x^43 */
        den[7]  = -1;  /* -x^7  */
        den[0]  =  1;  /* +1    */

        /* Long division num / den  →  quotient of degree 252. */
        const int degN = 302;
        const int degD = 50;
        const int degQ = degN - degD;    /* 252 */

        seq_t Q[253] = {0};

        for (int i = degQ; i >= 0; --i) {
            Q[i] = num[degD + i];
            if (Q[i] == 0) continue;
            for (int j = 0; j <= degD; ++j) {
                num[j + i] -= Q[i] * den[j];
            }
        }

        /* Store the coefficients Φ_301(x) = Σ φ(k) x^k. */
        for (int k = 0; k <= degQ; ++k) {
            phi[k] = Q[k];
        }

        /* Compute the first 301 terms of the inverse series via the
           convolution relation: a * Φ = 1.  Thus
               a(0) = 1
           and for n ≥ 1
               a(n) = – Σ_{k=1}^{min(n,252)} φ(k) * a(n–k).  */
        vals[0] = 1;
        for (int i = 1; i < 301; ++i) {
            seq_t acc = 0;
            int upto = (i < 252 ? i : 252);
            for (int k = 1; k <= upto; ++k) {
                acc -= phi[k] * vals[i - k];
            }
            vals[i] = acc;
        }

        init = true;
    }

    /* Periodic extension: a(n) = a(n mod 301). */
    return vals[(size_t)(n % 301u)];
}
