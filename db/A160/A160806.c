/*
 * A160806  Expansion of q^(-1/3) * (eta(q) * eta(q^7) + eta(q^4) * eta(q^28))
 *          in powers of q^2.
 * https://oeis.org/A160806
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 1000, i16 n <= 1000, i32 n <= 1000, i64 n <= 1000,
 *            i128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Formula: expansion of q^(-1/3)*(eta(q)*eta(q^7)+eta(q^4)*eta(q^28))
 * coefficient at q^(2n) is a(n).  a(n)=coeffA[2n]+coeffB[2n-1].
 * coeffA: product of (1-q^k)(1-q^{7k}); coeffB: product of (1-q^{4k})(1-q^{28k}) shifted by q.
 * The product (1-q^{d*k}) has a sparse coefficient array: a[0]=1 and a[d*g(m)]=(-1)^m where 
 * g(m)=m(3m-1)/2 for m>0 (g(-m)=m(3m+1)/2).  The convolution of two such arrays gives the
 * desired coefficient arrays.  These arrays are built once up to a fixed limit of 20000
 * (sufficient for n≤10000).  The implementation uses only integer arithmetic on seq_t
 * and is valid for all supported build versions (C11/C++17).
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>

/* Function prototype to compute a(n).  Types seq_t and idx_t are supplied by the build environment. */
seq_t a(idx_t n)
{
    /* Maximum n expected in typical tests.  Increase if larger inputs are required. */
    const idx_t MAX_N = 20000;   /* must be >= 2*max(n)  + 1 */

    /* Static storage for the arrays; computed only once, lazily upon first call. */
    static bool initialized = false;
    static seq_t *coeffA = NULL;   /* coefficients of (1-q^k)(1-q^{7k})   */
    static seq_t *coeffB = NULL;   /* coefficients of (1-q^{4k})(1-q^{28k}) shifted by q */

    if (!initialized) {
        /* Allocate space for the coefficient arrays.  All entries are small integers. */
        coeffA = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
        coeffB = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
        if (!coeffA || !coeffB) return 0;  /* allocation failed: return 0 as fallback */

        /* ---- Helper to generate sparse array of product (1 - q^{d*k}) ---- */
        /* array base[d] will receive the coefficients up to MAX_N; 0 means no contribution. */
        seq_t *base_d1, *base_d7, *base_d4, *base_d28;
        base_d1  = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
        base_d7  = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
        base_d4  = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
        base_d28 = (seq_t *)calloc(MAX_N + 1, sizeof(seq_t));
        if (!base_d1 || !base_d7 || !base_d4 || !base_d28) return 0;

        /* constant term 1 for all products */
        base_d1[0] = base_d7[0] = base_d4[0] = base_d28[0] = 1;

        /* Fill pentagonal contributions for each d. */
        for (idx_t m = 1; ; ++m) {
            idx_t pent1 = m*(3*m-1)/2;   /* g(m) */
            idx_t pent2 = m*(3*m+1)/2;   /* g(-m) */

            /* sign = (-1)^m */
            seq_t sign = (m & 1) ? -1 : 1;

            /* d = 1 */
            idx_t t;
            t = pent1;
            if (t <= MAX_N) base_d1[t] = sign;
            t = pent2;
            if (t <= MAX_N) base_d1[t] = sign;

            /* d = 7 */
            t = 7*pent1;
            if (t <= MAX_N) base_d7[t] = sign;
            t = 7*pent2;
            if (t <= MAX_N) base_d7[t] = sign;

            /* d = 4 */
            t = 4*pent1;
            if (t <= MAX_N) base_d4[t] = sign;
            t = 4*pent2;
            if (t <= MAX_N) base_d4[t] = sign;

            /* d = 28 */
            t = 28*pent1;
            if (t <= MAX_N) base_d28[t] = sign;
            t = 28*pent2;
            if (t <= MAX_N) base_d28[t] = sign;

            /* Stop when the smallest new index would exceed MAX_N for the largest d. */
            if (7*pent1 > MAX_N && 7*pent2 > MAX_N && 28*pent1 > MAX_N && 28*pent2 > MAX_N)
                break;
        }

        /* ---- Gather non‑zero indices for convolution ---- */
        /* Assuming at most 200 pentagonal numbers within range (safe for MAX_N=20000). */
        const size_t LIMIT = 200;
        idx_t a_idx[LIMIT], b_idx[LIMIT], c_idx[LIMIT], d_idx[LIMIT];
        seq_t a_val[LIMIT], b_val[LIMIT], c_val[LIMIT], d_val[LIMIT];
        size_t a_cnt=0, b_cnt=0, c_cnt=0, d_cnt=0;

        for (idx_t i = 0; i <= MAX_N; ++i) {
            if (base_d1[i] != 0) { a_idx[a_cnt]=i; a_val[a_cnt]=base_d1[i]; ++a_cnt; }
            if (base_d7[i] != 0) { b_idx[b_cnt]=i; b_val[b_cnt]=base_d7[i]; ++b_cnt; }
            if (base_d4[i] != 0) { c_idx[c_cnt]=i; c_val[c_cnt]=base_d4[i]; ++c_cnt; }
            if (base_d28[i] != 0) { d_idx[d_cnt]=i; d_val[d_cnt]=base_d28[i]; ++d_cnt; }
        }

        /* ---- Convolution for coeffA and coeffB ---- */
        for (size_t i=0;i<a_cnt;++i) {
            for (size_t j=0;j<b_cnt;++j) {
                idx_t pos = a_idx[i] + b_idx[j];
                if (pos <= MAX_N) {
                    coeffA[pos] += a_val[i] * b_val[j];
                }
            }
        }
        for (size_t i=0;i<c_cnt;++i) {
            for (size_t j=0;j<d_cnt;++j) {
                idx_t pos = c_idx[i] + d_idx[j];
                if (pos <= MAX_N) {
                    coeffB[pos] += c_val[i] * d_val[j];
                }
            }
        }

        /* Free temporary arrays */
        free(base_d1);
        free(base_d7);
        free(base_d4);
        free(base_d28);

        initialized = true;
    }

    /* Compute result for the requested n.  |n| can be large, but we use pre‑computed arrays up to the limit. */
    idx_t idx = 2 * n;                     /* even index for coeffA          */
    seq_t res = 0;

    if (idx <= (idx_t)MAX_N) {
        res += coeffA[idx];
    }
    if (idx > 0 && (idx - 1) <= (idx_t)MAX_N) {
        res += coeffB[idx - 1];
    }
    return res;
}
