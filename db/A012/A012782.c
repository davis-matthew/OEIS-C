/*
 * A012782  exp(sec(x)*arcsin(x))=1+x+1/2!*x^2+5/3!*x^3+17/4!*x^4+85/5!*x^5...
 * https://oeis.org/A012782
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the integer n! · [xⁿ] exp(sec(x)·arcsin(x)). The
 *            recurrence used is a(0) = 1 a(m) = ∑_{k=0}^{m-1} binom(m-1,k) ·
 *            a(k) · c(m‑k) (m ≥ 1) where c(k) are the integer coefficients of
 *            the EGF c(k) = k! · [xᵏ] (sec(x)·arcsin(x)). c(k) is obtained by
 *            the convolution c(n) = ∑_{i=0}^{n} binom(n,i)...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 13, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* --------------------------------------------------------------------
       static cache for all intermediate sequences
    -------------------------------------------------------------------- */
    static idx_t cache_len = 0;      /* number of valid indices (0 … cache_len-1) */
    static seq_t *S = NULL;          /* Euler secant numbers multiplied by factorials */
    static seq_t *B = NULL;          /* arcsin coefficients multiplied by factorials */
    static seq_t *C = NULL;          /* convolution of S and B (coefficients of C) */
    static seq_t *D = NULL;          /* desired sequence (a(n)) */

    /* extend cache if required ------------------------------------------------ */
    if (n + 1 > cache_len) {
        idx_t old_len  = cache_len;
        idx_t new_len  = n + 1;
        cache_len = new_len;

        S = (seq_t *)realloc(S, new_len * sizeof(seq_t));
        B = (seq_t *)realloc(B, new_len * sizeof(seq_t));
        C = (seq_t *)realloc(C, new_len * sizeof(seq_t));
        D = (seq_t *)realloc(D, new_len * sizeof(seq_t));

        for (idx_t i = old_len; i < new_len; ++i) {
            S[i] = 0;
            B[i] = 0;
            C[i] = 0;
            D[i] = 0;
        }

        /* ------------------------------------------------------------------
           initialise base cases:
           S[0] = 1 (sec(0)=1)
           B[1] = 1 (arcsin(x)=x + …  → coefficient of x¹ = 1)
        ------------------------------------------------------------------ */
        if (S[0] == 0) S[0] = 1;
        if (new_len > 1 && B[1] == 0) B[1] = 1;

        /* ------------------------------------------------------------------
           compute Euler secant numbers S (non‑zero only for even indices)
        ------------------------------------------------------------------ */
        for (idx_t m = 1; m < new_len; ++m) {
            if (S[m] != 0) continue;               /* already initialised for m=0 */
            seq_t sum = 0;
            for (idx_t k = 0; k <= m-1; ++k) {
                if (((m - k) & 1) == 0) {           /* m‑k even → cos term appears */
                    idx_t exp = (m - k) >> 1;        /* (m-k)/2  */
                    seq_t term = binom(m, k) * S[k];
                    if (exp & 1)                    /* odd exponent → negative sign */
                        sum += term;
                    else                            /* even exponent → positive sign */
                        sum -= term;
                }
            }
            S[m] = sum;
        }

        /* ------------------------------------------------------------------
           compute arcsin coefficient sequence B (only odd indices)
           recurrence:  b_{2m+1} = b_{2m−1} * (2m−1)²   with b₁ = 1
        ------------------------------------------------------------------ */
        for (idx_t m = 1; 2*m + 1 < new_len; ++m) {
            idx_t odd_cur  = 2*m + 1;
            idx_t odd_prev = 2*(m-1) + 1;        /* 2m - 1  */
            seq_t factor = ( (2*m - 1) * (2*m - 1) );   /* (2m-1)²  */
            B[odd_cur] = B[odd_prev] * factor;
        }

        /* ------------------------------------------------------------------
           convolution C = S * B  (coefficients of the product sec·arcsin)
        ------------------------------------------------------------------ */
        for (idx_t i = 0; i < new_len; ++i) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= i; ++k) {
                if (S[k] == 0 || B[i - k] == 0) continue;
                seq_t term = binom(i, k);
                term = term * S[k];
                term = term * B[i - k];
                sum += term;
            }
            C[i] = sum;
        }

        /* ------------------------------------------------------------------
           recurrence for D (the desired sequence)
           a(0)=1; for m≥1:
               a(m) = Σ_{k=0}^{m-1} binom(m-1,k) * a(k) * c(m-k)
        ------------------------------------------------------------------ */
        D[0] = 1;
        for (idx_t m = 1; m < new_len; ++m) {
            seq_t sum = 0;
            for (idx_t k = 0; k <= m-1; ++k) {
                seq_t term = binom(m - 1, k);
                term = term * D[k];
                term = term * C[m - k];
                sum += term;
            }
            D[m] = sum;
        }
    }

    return D[n];
}
