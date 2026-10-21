/*
 * A215704  n-th derivative of ((x^x)^x)^x at x=1.
 * https://oeis.org/A215704
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   Let g(x) = x^3*ln(x). Define g_k = g^(k)(1) = k! *
 *            Σ_{i=0}^{min(3,k-1)} [ binom(3,i) * (-1)^{k-i+1} / (k-i) ] .
 *            Then the sequence satisfies a(0) = 1, a(n) = Σ_{k=1..n}
 *            binom(n-1,k-1) * g_k * a(n-k) (n ≥ 1). This recurrence follows
 *            from the derivative of e^{g(x)}
 * Range:     i8 n <= 3, i16 n <= 6, i32 n <= 11, i64 n <= 19, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#if defined(__cplusplus)
#include <vector>
#endif
#include <stdlib.h>   /* malloc, free */
#include <stdint.h>   /* for uintptr_t if needed */

/* Constants for binom(3,i) */
static const seq_t CONST_I[4] = { (seq_t)1, (seq_t)3, (seq_t)3, (seq_t)1 };

/* Compute a(n) */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    size_t N = (size_t)n; /* for array bounds */

    /* Allocate factorial array */
#if defined(__cplusplus)
    std::vector<seq_t> fact(N + 1);
#else
    seq_t *fact = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!fact) return (seq_t)0;
#endif

    fact[0] = (seq_t)1;
    for (size_t i = 1; i <= N; ++i)
        fact[i] = fact[i - 1] * (seq_t)i;

    /* Compute g_k (k = 1..N) */
#if defined(__cplusplus)
    std::vector<seq_t> g(N + 1);
#else
    seq_t *g = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!g) { free(fact); return (seq_t)0; }
#endif

    for (size_t k = 1; k <= N; ++k) {
        seq_t sum = (seq_t)0;
        size_t max_i = (k > 3 ? 3 : (size_t)(k - 1));
        for (size_t i = 0; i <= max_i; ++i) {
            seq_t ai = CONST_I[i];
            /* sign = (-1)^(k-i+1) */
            int sign = (((int)(k - i + 1)) & 1) ? -1 : 1;
            seq_t denom = (seq_t)(k - i);
            /* fact[k] / denom is exact */
            seq_t term = fact[k] / denom;
            term = term * ai;
            if (sign < 0)
                term = -term;
            sum += term;
        }
        g[k] = sum;
    }

    /* Compute a[0..N] */
#if defined(__cplusplus)
    std::vector<seq_t> A(N + 1);
#else
    seq_t *A = (seq_t *)malloc((N + 1) * sizeof(seq_t));
    if (!A) { free(g); free(fact); return (seq_t)0; }
#endif

    A[0] = (seq_t)1;
    for (size_t m = 1; m <= N; ++m) {
        seq_t sum = (seq_t)0;
        seq_t binom = (seq_t)1; /* binom(m-1,0) */
        for (size_t k = 1; k <= m; ++k) {
            if (k > 1) {
                seq_t num = (seq_t)(m - 1 - (k - 2));
                seq_t den = (seq_t)(k - 1);
                binom = binom * num / den;   /* exact division */
            }
            seq_t term = A[m - k];
            term = term * g[k];
            term = term * binom;
            sum += term;
        }
        A[m] = sum;
    }

    seq_t result = A[N];

    /* Free allocated memory (malloc version only) */
#if !defined(__cplusplus)
    free(g);
    free(fact);
    free(A);
#endif

    return result;
}
