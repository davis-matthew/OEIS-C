/*
 * A009336  Expansion of log(1+sin(x))/cosh(x).
 * https://oeis.org/A009336
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, easy
 * Formula:   Let f(x) = log(1+sin(x))/cosh(x). a(n) = n! * [x^n] f(x). The
 *            following code computes a(n) for all n≥0 by power‑series
 *            manipulation without using floating‑point arithmetic
 * Range:     i8 n <= 6, i16 n <= 9, i32 n <= 15, i64 n <= 22, i128 n <= 22,
 *            big any n
 * Helpers:   alloc_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* includes for the utilities that allocate sequences */
#include "../utils/alloc_seq.h"
#include <stddef.h>   /* for size_t and NULL */

/* external linkage function */
seq_t a(idx_t n)
{
    /* Allocate space for intermediate sequences */
    seq_t *W = alloc_seq(n + 1);   /* 1/cosh(x) coefficients × n!   */
    seq_t *T = alloc_seq(n + 1);   /* log(1+sin(x)) coefficients × n!   */
    seq_t *U = alloc_seq(n + 1);   /* 1/cosh(x) coefficients × n! (same as W) but computed independently */

    /* -----------------------------------------------------------------
        Compute W sequence:
        W[0] = 1
        For n ≥ 1 :
           W[n] = - Σ_{k=0}^{n-1}  binom(n,k) * S[n-k] * W[k]
        where S[m] = { 0 if m even,
                       (-1)^{(m-1)/2} if m odd }
       ----------------------------------------------------------------- */
    W[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = 0;
        seq_t bin = 1;   /* binom(i,0) */
        for (idx_t k = 0; k < i; ++k)
        {
            idx_t idx = i - k;
            if (idx & 1)              /* odd index */
            {
                int s = ((idx % 4) == 1) ? 1 : -1;
                sum += bin * (seq_t)s * W[k];
            }
            if (k + 1 < i)            /* update binom(i,k+1) */
                bin = bin * (seq_t)(i - k) / (seq_t)(k + 1);
        }
        W[i] = -sum;
    }

    /* -----------------------------------------------------------------
        Compute T sequence:
        T[0] = 0
        For n ≥ 1 :
           T[n] = Σ_{k=0}^{n-1}  binom(n-1,k) * S[k+1] * W[n-1-k]
       ----------------------------------------------------------------- */
    T[0] = (seq_t)0;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = 0;
        seq_t bin = 1;   /* binom(i-1,0) */
        idx_t m = i - 1; /* constant in this loop */
        for (idx_t k = 0; k <= m; ++k)
        {
            idx_t idx = k + 1;
            if (idx & 1)               /* odd index */
            {
                int s = ((idx % 4) == 1) ? 1 : -1;
                sum += bin * (seq_t)s * W[m - k];
            }
            if (k + 1 <= m)            /* update binom(m,k+1) */
                bin = bin * (seq_t)(m - k) / (seq_t)(k + 1);
        }
        T[i] = sum;
    }

    /* -----------------------------------------------------------------
        Compute U sequence (inverse of cosh(x)):
        U[0] = 1
        For n ≥ 1 :
           U[n] = - Σ_{k=0}^{n-1}  binom(n,k) * Q[n-k] * U[k]
        where Q[m] = 1 if m even, 0 otherwise.
       ----------------------------------------------------------------- */
    U[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
    {
        seq_t sum = 0;
        seq_t bin = 1;   /* binom(i,0) */
        for (idx_t k = 0; k < i; ++k)
        {
            idx_t idx = i - k;
            if ((idx & 1) == 0)        /* even index */
                sum += bin * U[k];
            if (k + 1 < i)            /* update binom(i,k+1) */
                bin = bin * (seq_t)(i - k) / (seq_t)(k + 1);
        }
        U[i] = -sum;
    }

    /* -----------------------------------------------------------------
        Finally compute a[n]:
           a[n] = Σ_{k=0}^{n}  binom(n,k) * T[k] * U[n-k]
       ----------------------------------------------------------------- */
    seq_t an = 0;
    seq_t bin = 1;   /* binom(n,0) */
    for (idx_t k = 0; k <= n; ++k)
    {
        an += bin * T[k] * U[n - k];
        if (k < n)                /* update binom(n,k+1) */
            bin = bin * (seq_t)(n - k) / (seq_t)(k + 1);
    }

    /* Free allocated memory (optional, omitted to avoid missing header) */
    /* free_seq_arr(W); free_seq_arr(T); free_seq_arr(U); */

    return an;
}
