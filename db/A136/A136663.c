/*
 * A136663  Triangle of coefficients of the Pascal sum of A053120 Chebyshev's
 *          T(n, x) polynomials :p(x,n)=2*x*p(x,n-1)-p(x,n-2);
 *          pp(x,n)=Sum[Binomial[n,m]*p(x,m),{m,0,n}].
 * https://oeis.org/A136663
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  uned, tabl, sign
 * Formula:   not stated
 * Range:     i8 n <= 31, i16 n <= 66, i32 n <= 66, i64 n <= 66, i128 n <= 66,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Triangle of coefficients of the Pascal sum of Chebyshev's T(n,x) polynomials.
   a(n) is the entry of the triangle read left‑to‑right, row by row.
   Row r (1‑based) contains the coefficients of pp(x,r-1) and has r entries.
   For a given linear index n, let r be the smallest integer with
   r(r+1)/2 ≥ n and k = n – r(r-1)/2 – 1.  Then a(n) is the coefficient of
   degree k in pp(x,r-1):
          a(n) = Σ_{m=k}^{r-1} binom(r-1,m) * coeff_k(p(x,m))
   where p(x,m) satisfies p(x,0)=1, p(x,1)=x,
   p(x,m)=2x*p(x,m-1)-p(x,m-2).  The coefficient of
   degree k of p(x,m) is computed recursively using only elements ≤k. */
#include <stdlib.h>
#include <stdint.h>

/* Assume seq_t and idx_t are defined by the build system. */
seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                         /* undefined for zero index */

    /* Find the row r such that  r(r+1)/2 ≥ n. */
    idx_t r = 1;
    while (((seq_t)r * (r + 1)) / 2 < n)
        ++r;

    /* Parameters for the computation */
    const idx_t N = r - 1;                      /* the polynomial index */
    const idx_t k = n - (r * (r - 1)) / 2 - 1;  /* position inside the row */
    const size_t len = (size_t)k + 1;           /* number of coefficients we need */

    /* Allocate coefficient arrays */
    seq_t *prev2 = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *prev1 = (seq_t *)calloc(len, sizeof(seq_t));
    seq_t *curr  = (seq_t *)calloc(len, sizeof(seq_t));
    if (!prev2 || !prev1 || !curr) {
        /* Allocation failed – return 0 as a safe fallback. */
        free(prev2); free(prev1); free(curr);
        return 0;
    }

    /* p(x,0) = 1  → prev1[0] = 1  */
    prev1[0] = 1;

    seq_t ans   = 0;
    seq_t binom = 1;   /* binom = C(N,0) */

    /* m = 0 contribution */
    ans += binom * prev1[k];

    /* m = 1 contribution (if applicable) */
    if (N >= 1) {
        binom = binom * (seq_t)N;   /* C(N,1) */
        for (size_t t = 0; t < len; ++t)
            curr[t] = 0;
        if (k >= 1)
            curr[1] = 1;            /* p(x,1) = x */
        ans += binom * curr[k];
        /* rotate buffers */
        seq_t *tmp  = prev2;
        prev2      = prev1;
        prev1      = curr;
        curr       = tmp;
    }

    /* m = 2 … N contributions */
    for (idx_t m = 2; m <= N; ++m) {
        binom = binom * (seq_t)(N - m + 1) / (seq_t)m;   /* C(N,m) */
        for (size_t t = 0; t < len; ++t) {
            seq_t left  = (t > 0) ? (seq_t)2 * prev1[t - 1] : 0;
            curr[t] = left - prev2[t];                    /* p(x,m) coefficient */
        }
        ans += binom * curr[k];
        /* rotate buffers */
        seq_t *tmp  = prev2;
        prev2      = prev1;
        prev1      = curr;
        curr       = tmp;
    }

    /* Free allocated memory */
    free(prev2);
    free(prev1);
    free(curr);

    return ans;
}
