/*
 * A013246  E.g.f.: sin(log(x+1)-arctan(x)) =
 *          -1/2!*x^2+4/3!*x^3-6/4!*x^4-105/6!*x^6...
 * https://oeis.org/A013246
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 6, i16 n <= 8, i32 n <= 13, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A013246: a(n) = coefficient of x^n in the EGF sin(log(1+x)-atan(x))
   where            ∑_{n≥0} a(n) x^n / n! = sin(log(1+x)-atan(x)).
   The recurrence below uses the differential equation
      d/dx sin(h) = cos(h) · h',
   with h(x)=log(1+x)-atan(x).  Coefficients are computed in the
   exponential generating function (EGF) notation.  All arithmetic
   is performed with the provided integer type seq_t. */

#include <stdlib.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* a(0) = 0 */

    /* Allocate arrays for sin and cos coefficients */
    seq_t *U = (seq_t*)malloc((n + 1) * sizeof(seq_t)); /* sin coefficients */
    seq_t *V = (seq_t*)malloc((n + 1) * sizeof(seq_t)); /* cos coefficients */
    if (!U || !V) { free(U); free(V); return 0; }

    /* h'(x) = 1/(1+x) - 1/(1+x^2) */
    /* W[k] is the coefficient of x^k in h'(x) */
    seq_t *W = (seq_t*)malloc((n + 1) * sizeof(seq_t));
    if (!W) { free(U); free(V); return 0; }

    for (idx_t k = 0; k <= n; ++k) {
        int sign = (k & 1) ? -1 : 1;          /* (-1)^k   */
        if ((k & 1) == 0) {                  /* even k   */
            if (((k >> 1) & 1) == 1)          /* (k/2) odd */
                W[k] = (seq_t)(2 * sign);
            else
                W[k] = 0;
        } else {                              /* odd k    */
            /* here w = (-1)^k = sign (since sign is -1 for odd k) */
            W[k] = (seq_t)(sign);
        }
    }

    /* Initial values: sin(0)=0, cos(0)=1 */
    U[0] = 0;
    V[0] = 1;

    /* Main recurrence loop: compute U[n] and V[n] for n≥1 */
    for (idx_t m = 0; m < n; ++m) {
        seq_t sumU = 0;
        seq_t sumV = 0;
        seq_t prod = 1;                     /* prod = m! / k! for current k */

        for (idx_t k = m; ; --k) {
            seq_t w = W[m - k];             /* coefficient of x^{m-k} in h' */
            seq_t termU = V[k] * w * prod;  /* contribution to a_{m+1} */
            seq_t termV = U[k] * w * prod;  /* contribution to b_{m+1} */
            sumU += termU;
            sumV += termV;
            if (k == 0) break;
            prod *= (seq_t)k;                /* update prod for next k-1 */
        }

        U[m + 1] = sumU;                     /* a_{m+1} = sumU */
        V[m + 1] = -sumV;                    /* b_{m+1} = -sumV */
    }

    seq_t result = U[n];

    free(U);
    free(V);
    free(W);
    return result;
}
