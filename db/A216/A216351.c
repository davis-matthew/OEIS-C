/*
 * A216351  n-th derivative of x^(x^(n-1)) at x=1.
 * https://oeis.org/A216351
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n! * [x^n] (x+1)^((x+1)^(n-1))
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 8, u64 n <= 13, u128 n <= 22,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

#if __cplusplus
/* In C++ big‑int types need proper construction */
static seq_t* alloc_arr(size_t n) { return new seq_t[n](); }
static void  dealloc_arr(seq_t* p) { delete [] p; }
#else
/* In C unsigned integer types have no constructor */
static seq_t* alloc_arr(size_t n) { return (seq_t*)malloc(n * sizeof(seq_t)); }
static void  dealloc_arr(seq_t* p) { free(p); }
#endif

seq_t a(idx_t n)
{
    /* a(0) = 1 */
    if (n == 0)
        return (seq_t)1;

    /* Allocate temporary arrays */
    seq_t *fall = alloc_arr((size_t)n);          /* P(n‑1,j), 0 ≤ j < n   */
    seq_t *fact = alloc_arr((size_t)(n + 1));    /* factorials 0! … n!    */
    seq_t *u    = alloc_arr((size_t)(n + 1));    /* u_k, 1 ≤ k ≤ n        */
    seq_t *B    = alloc_arr((size_t)(n + 1));    /* Bell numbers 0 … n    */

    if (!fall || !fact || !u || !B) {
        dealloc_arr(fall); dealloc_arr(fact);
        dealloc_arr(u);   dealloc_arr(B);
        return (seq_t)0;   /* out of memory */
    }

    /* factorials 0! … n! */
    fact[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        fact[i] = fact[i - 1] * (idx_t)i;

    /* falling factorials P(n‑1,j) = (n‑1)!/(n‑1‑j)!  (j = 0 … n‑1) */
    fall[0] = (seq_t)1;
    for (idx_t j = 1; j < n; ++j)
        fall[j] = fall[j - 1] * (idx_t)(n - j);

    /* u_k = Σ_{j=0}^{k‑1} (-1)^{k‑j‑1}·P(n‑1,j)·(k‑j‑1)!·C(k,j)   */
    for (idx_t k = 1; k <= n; ++k) {
        seq_t pos = (seq_t)0, neg = (seq_t)0;
        seq_t bin = (seq_t)1;           /* C(k,0)                                    */
        for (idx_t j = 0; j < k; ++j) {
            seq_t term = bin;
            term *= fall[j];
            term *= fact[k - j - 1];

            if (((k - j - 1) & 1u) != 0)   /* odd -> negative component         */
                neg += term;
            else
                pos += term;

            /* update binomial coefficient C(k,j+1) */
            if (j + 1 < k) {
                bin *= (idx_t)(k - j);
                bin /= (idx_t)(j + 1);   /* exact division                        */
            }
        }
        /* pos ≥ neg for all k, so subtraction is safe            */
        u[k] = pos - neg;
    }

    /* Bell polynomials:  B_0 = 1,
     *                    B_m = Σ_{k=1}^{m} C(m‑1,k‑1)·u_k·B_{m‑k}  (m ≥ 1)   */
    B[0] = (seq_t)1;
    for (idx_t m = 1; m <= n; ++m) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;               /* C(m-1,0)                              */
        for (idx_t k = 1; k <= m; ++k) {
            seq_t term = bin;
            term *= u[k];
            term *= B[m - k];
            sum += term;

            /* update binomial coefficient C(m-1,k) */
            if (k < m) {
                bin *= (idx_t)(m - k);
                bin /= (idx_t)k;           /* exact division                        */
            }
        }
        B[m] = sum;
    }

    seq_t result = B[n];

    /* Free temporary arrays */
    dealloc_arr(fall);
    dealloc_arr(fact);
    dealloc_arr(u);
    dealloc_arr(B);
    return result;
}
