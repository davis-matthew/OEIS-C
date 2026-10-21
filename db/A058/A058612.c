/*
 * A058612  McKay-Thompson series of class 30A for Monster.
 * https://oeis.org/A058612
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = coefficient of q^n in the McKay–Thompson series T30A = 3
 *            + ((η(q)·η(q⁶)·η(q¹⁰)·η(q¹⁵))/(η(q²)·η(q³)·η(q⁵)·η(q³⁰)))³ The
 *            series has offset -1, i.e. a(-1) is the first listed term
 * Range:     i8 n <= 29, i16 n <= 88, i32 n <= 279, i64 n <= 957,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>

/* constants for the η–product */
static const idx_t NUM_K[4] = {1, 6, 10, 15};   /* numerator ks   */
static const idx_t DEN_K[4] = {2, 3, 5, 30};    /* denominator ks */

/* ---------------------------------------------------------------------- */
/* Build the power series P(q) = ∏_{t≥1}(1‑qᵗ)^E[t] truncated to degree
   deg = max_n + 1 (i.e. coefficients of q⁰ … q^deg are computed).
   The returned array P[0…deg] contains those coefficients.
   Caller is responsible for freeing the array.                      */
/* ---------------------------------------------------------------------- */
static void build_P(idx_t max_n, seq_t **P_out)
{
    idx_t deg = max_n + 1;              /* highest exponent needed   */
    size_t len = (size_t)(deg + 1);     /* array length              */

    seq_t *P = (seq_t *)malloc(len * sizeof(seq_t));
    if (!P) exit(1);                     /* out–of–memory            */

    /* initialise P(q) = 1 */
    for (size_t i = 0; i < len; ++i) P[i] = (seq_t)0;
    P[0] = (seq_t)1;

    /* iterate over each factor (1‑qᵗ)^{E[t]} */
    for (idx_t t = 1; t <= deg; ++t) {
        int cnt_num = 0, cnt_den = 0;
        for (int k = 0; k < 4; ++k) if (t % NUM_K[k] == 0) ++cnt_num;
        for (int k = 0; k < 4; ++k) if (t % DEN_K[k] == 0) ++cnt_den;
        int e = 3 * (cnt_num - cnt_den);          /* exponent for this t   */

        if (e > 0) {              /* multiply by (1‑qᵗ)^e */
            for (int rep = 0; rep < e; ++rep)
                for (idx_t i = deg; i >= t; --i)
                    P[(size_t)i] -= P[(size_t)(i - t)];
        } else if (e < 0) {        /* divide by (1‑qᵗ)^{‑e} */
            int neg = -e;
            for (int rep = 0; rep < neg; ++rep)
                for (idx_t i = t; i <= deg; ++i)
                    P[(size_t)i] += P[(size_t)(i - t)];
        }
    }

    *P_out = P;
}

/* ---------------------------------------------------------------------- */
/* Cached data: the coefficients of the base series P(q).                */
/* ---------------------------------------------------------------------- */
static seq_t *P_cache      = NULL;     /* coefficients of P(q) */
static idx_t  cache_max_n  = -2;       /* highest n for which cache is valid */

/* ---------------------------------------------------------------------- */
/* Sequence entry point.  a(n) returns the coefficient of q^n in
   the McKay–Thompson series.                                               */
/* ---------------------------------------------------------------------- */
seq_t a(idx_t n)
{
    if (n < -1)                 /* undefined below offset */
        return (seq_t)0;

    /* Rebuild cache if we need higher indices */
    if (n > cache_max_n) {
        if (P_cache) free(P_cache);
        build_P(n, &P_cache);
        cache_max_n = n;
    }

    /* Q‑shift: the series is 3 + q⁻¹·P(q)
       coefficient of q^n is P[n+1]  (except for n = 0 where we add 3) */
    if (n == -1)                /* coefficient of q^(-1) */
        return P_cache[0];      /* = 1 */
    else if (n == 0)            /* constant term: P[1] + 3 */
        return P_cache[1] + (seq_t)3;
    else
        return P_cache[(size_t)(n + 1)];
}
