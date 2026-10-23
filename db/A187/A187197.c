/*
 * A187197  McKay-Thompson series of class 12E for the Monster group with a(0)
 *          = 2.
 * https://oeis.org/A187197
 *
 * Offset:    -1    (a(-1) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 18, i16 n <= 60, i32 n <= 208, i64 n <= 738,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A187197: coefficients of ((psi(q)*psi(q^3))/(psi(q^2)*psi(q^6)))^2 / q.
   a(n) is the coefficient of q^(n+1) in the power series of the square
   of the quotient (psi(q)*psi(q^3))/(psi(q^2)*psi(q^6)).  The function
   below builds the necessary coefficients up to the required index and
   returns the requested term.  All arithmetic is performed in type
   seq_t, and the implementation works for every supported integer
   type, including arbitrary‑precision classes.  The routine is
   O(n·√n) and caches results, so repeated calls are fast.  The code is
   valid C11 and C++17.  */

#include <stdbool.h>
#include <stddef.h>
#include <stdlib.h>

static seq_t *H_cache = NULL;      /* coefficients of S(q)^2,  H_cache[n] = coeff of q^n  */
static size_t cache_cap = 0;       /* current number of stored coefficients (>=1) */

/* recompute coefficient array H up to index need_idx (inclusive) */
static void recompute(size_t need_idx)
{
    size_t sz = need_idx + 1;           /* number of coefficients 0..need_idx */

    /* temporary arrays for G and D */
    seq_t *G = (seq_t*)calloc(sz, sizeof(seq_t));
    seq_t *D = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!G || !D)
        abort();

    /* lists of exponents for the four ψ-series */
    size_t *exp_q   = NULL;  size_t cnt_q   = 0;
    size_t *exp_q3  = NULL;  size_t cnt_q3  = 0;
    size_t *exp_q2  = NULL;  size_t cnt_q2  = 0;
    size_t *exp_q6  = NULL;  size_t cnt_q6  = 0;

    /* generate all relevant exponents up to need_idx */
    for (size_t k = 0; ; ++k)
    {
        size_t t = k * (k + 1) / 2;
        if (t > need_idx) break;

        exp_q   = (size_t*)realloc(exp_q,   (cnt_q+1)  * sizeof(size_t));
        exp_q[cnt_q++] = t;
        size_t t3 = t * 3;
        if (t3 <= need_idx)
        {
            exp_q3  = (size_t*)realloc(exp_q3,  (cnt_q3+1) * sizeof(size_t));
            exp_q3[cnt_q3++] = t3;
        }
        size_t t2 = k * (k + 1);
        if (t2 <= need_idx)
        {
            exp_q2  = (size_t*)realloc(exp_q2,  (cnt_q2+1) * sizeof(size_t));
            exp_q2[cnt_q2++] = t2;
        }
        size_t t6 = t2 * 3;
        if (t6 <= need_idx)
        {
            exp_q6  = (size_t*)realloc(exp_q6,  (cnt_q6+1) * sizeof(size_t));
            exp_q6[cnt_q6++] = t6;
        }
    }

    /* G = ψ(q) * ψ(q^3) */
    for (size_t i = 0; i < cnt_q; ++i)
    {
        size_t e1 = exp_q[i];
        for (size_t j = 0; j < cnt_q3; ++j)
        {
            size_t total = e1 + exp_q3[j];
            if (total > need_idx) break;
            G[total] += (seq_t)1;
        }
    }

    /* D = ψ(q^2) * ψ(q^6) */
    for (size_t i = 0; i < cnt_q2; ++i)
    {
        size_t e1 = exp_q2[i];
        for (size_t j = 0; j < cnt_q6; ++j)
        {
            size_t total = e1 + exp_q6[j];
            if (total > need_idx) break;
            D[total] += (seq_t)1;
        }
    }

    free(exp_q); free(exp_q3); free(exp_q2); free(exp_q6);

    /* Dnz: indices i>0 where D[i] != 0 (D[0]==1 handled separately) */
    size_t *Dnz = (size_t*)calloc(need_idx + 1, sizeof(size_t));
    size_t Dnz_cnt = 0;
    for (size_t i = 1; i <= need_idx; ++i)
    {
        if (D[i] != (seq_t)0)
        {
            Dnz = (size_t*)realloc(Dnz, (Dnz_cnt+1) * sizeof(size_t));
            Dnz[Dnz_cnt++] = i;
        }
    }

    /* R = G / D  (the quotient series) */
    seq_t *R = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!R) abort();
    R[0] = G[0];                  /* G[0] = D[0] = 1 */

    for (size_t n = 1; n <= need_idx; ++n)
    {
        seq_t temp = G[n];
        for (size_t idx = 0; idx < Dnz_cnt; ++idx)
        {
            size_t k = Dnz[idx];
            if (k > n) break;
            temp -= D[k] * R[n - k];
        }
        R[n] = temp;
    }

    free(Dnz);
    free(G);
    free(D);

    /* Rnz: indices where R[i] != 0 */
    size_t *Rnz = (size_t*)calloc(need_idx + 1, sizeof(size_t));
    size_t Rnz_cnt = 0;
    for (size_t i = 0; i <= need_idx; ++i)
    {
        if (R[i] != (seq_t)0)
        {
            Rnz = (size_t*)realloc(Rnz, (Rnz_cnt+1) * sizeof(size_t));
            Rnz[Rnz_cnt++] = i;
        }
    }

    /* H = R * R   (convolution) */
    seq_t *H_new = (seq_t*)calloc(sz, sizeof(seq_t));
    if (!H_new) abort();

    for (size_t ai = 0; ai < Rnz_cnt; ++ai)
    {
        size_t i = Rnz[ai];
        seq_t Ri = R[i];
        for (size_t bi = ai; bi < Rnz_cnt; ++bi)
        {
            size_t j = Rnz[bi];
            size_t idx = i + j;
            if (idx > need_idx) break;
            seq_t prod = Ri * R[j];
            if (ai == bi)
                H_new[idx] += prod;
            else
                H_new[idx] += prod + prod;   /* each unordered pair counted twice */
        }
    }

    free(Rnz);
    free(R);

    /* install new cache */
    if (H_cache) free(H_cache);
    H_cache = H_new;
    cache_cap = sz;
}

/* public sequence function */
seq_t a(idx_t n)
{
    if (n < -1)                      /* behaviour before offset is irrelevant */
        return (seq_t)0;

    size_t need_idx = (size_t)(n + 1);   /* we need H_cache[need_idx] */
    if (need_idx + 1 > cache_cap)
        recompute(need_idx);

    return H_cache[need_idx];
}
