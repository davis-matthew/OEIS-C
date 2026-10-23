/*
 * A108482  Expansion of a modular function for Gamma(7).
 * https://oeis.org/A108482
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 56, i16 n <= 74, i32 n <= 74, i64 n <= 74, i128 n <= 74,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A108482: Expansion of a modular function for Γ(7)
    G.f.: ∏_{k>0} (1‑x^(7k‑3))(1‑x^(7k‑4))/((1‑x^(7k‑1))(1‑x^(7k‑6)))
    The coefficient a(n) is computed by expanding this infinite product
    up to the required degree n.  This implementation computes the
    product incrementally for each call and works both in C11 and C++17.  */

#include <stdlib.h>

static seq_t *coeff = NULL;          /* array of coefficients up to curmax   */
static idx_t  curmax = 0;            /* largest index already computed        */

static void recompute_upto(idx_t n)
{
#if defined(__cplusplus)
    /* allocate memory with C++ new[] */
    seq_t *newcoeff = new seq_t[n + 1];
#else
    /* allocate memory with plain malloc and cast */
    seq_t *newcoeff = (seq_t *)malloc((n + 1) * sizeof(seq_t));
#endif
    if (!newcoeff) { abort(); }

    /* initialise to zero */
    for (idx_t i = 0; i <= n; ++i) newcoeff[i] = 0;
    newcoeff[0] = 1;                         /* start of the product */

    /* multiply by each factor in the product up to degree n */
    for (idx_t k = 1; 7 * k - 6 <= n; ++k) {
        idx_t e1 = 7 * k - 3;   /* (1‑x^e1)   */
        idx_t e2 = 7 * k - 4;   /* (1‑x^e2)   */
        idx_t e3 = 7 * k - 1;   /* 1/(1‑x^e3) */
        idx_t e4 = 7 * k - 6;   /* 1/(1‑x^e4) */

        /* multiply by (1‑x^e1) */
        if (e1 <= n)
            for (idx_t i = n; i >= e1; --i)
                newcoeff[i] -= newcoeff[i - e1];

        /* multiply by (1‑x^e2) */
        if (e2 <= n)
            for (idx_t i = n; i >= e2; --i)
                newcoeff[i] -= newcoeff[i - e2];

        /* multiply by 1/(1‑x^e3) */
        if (e3 <= n)
            for (idx_t i = e3; i <= n; ++i)
                newcoeff[i] += newcoeff[i - e3];

        /* multiply by 1/(1‑x^e4) */
        if (e4 <= n)
            for (idx_t i = e4; i <= n; ++i)
                newcoeff[i] += newcoeff[i - e4];
    }

    /* free previous array (handle both C++ and C) */
#if defined(__cplusplus)
    delete[] coeff;
#else
    free(coeff);
#endif

    coeff  = newcoeff;
    curmax = n;
}

seq_t a(idx_t n)
{
    if (!coeff || n > curmax)
        recompute_upto(n);
    return coeff[n];
}
