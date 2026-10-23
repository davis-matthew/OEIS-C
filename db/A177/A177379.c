/*
 * A177379  E.g.f. satisfies: A(x) = 1/(1-x - x*log(A(x))).
 * https://oeis.org/A177379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) satisfies the implicit equation A(x) = 1 / (1 - x - x * log
 *            A(x)) (E.g.f.) The following recurrence only uses integer
 *            arithmetic and works for any build that provides the types seq_t
 *            and idx_t
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 9, u64 n <= 15, u128 n <= 17,
 *            big any n
 * Helpers:   seq_alloc_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/seq_alloc_array.h"
 
extern seq_t a(idx_t n)  /* external linkage as requested */
{
    /* static storage for the sequence and the auxiliary log‑coefficients */
    static seq_t *A = NULL;   /* a[0 … max] */
    static seq_t *L = NULL;   /* l[0 … max], l[0] = 0, l[n] = coeff of x^n/n! in log A */
    static idx_t max_computed = 0;   /* highest index that has been filled (>=0) */
    static size_t alloc_size = 0;    /* current allocation size */
 
    /* allocate new storage when needed */
    if (n + 1 > alloc_size) {
        size_t new_size = (size_t)(n + 1);
        seq_t *newA = seq_alloc_array(new_size);
        seq_t *newL = seq_alloc_array(new_size);
        if (alloc_size > 0) {
            for (size_t i = 0; i < alloc_size; ++i) {
                newA[i] = A[i];
                newL[i] = L[i];
            }
        }
        /* free old storage if we had it */
        if (alloc_size > 0) {
            free(A);
            free(L);
        }
        A = newA;
        L = newL;
        alloc_size = new_size;
 
        /* initialise base case */
        if (max_computed == 0) {
            A[0] = (seq_t)1;      /* a(0) = 1 */
            L[0] = (seq_t)0;      /* l(0) = 0   (log pad) */
        }
    }
 
    /* extend the known values up to n */
    for (idx_t m = max_computed + 1; m <= n; ++m) {
        /* ---------- compute a[m] ---------- */
        seq_t sum = 0;
        seq_t binom = 1;
        for (idx_t k = 1; k <= m; ++k) {
            /* update binomial coefficient C(m,k) */
            binom = binom * (seq_t)(m - k + 1) / (seq_t)k;
            /* term = l[k-1] * k * a[m-k] * binom */
            seq_t term = L[k - 1];
            term *= (seq_t)k;
            term *= A[m - k];
            term *= binom;
            sum += term;
        }
        A[m] = (seq_t)m * A[m - 1] + sum;
 
        /* ---------- compute l[m]  (coefficients of log A) ---------- */
        seq_t sum_l = 0;
        if (m >= 2) {                    /* l[1] is handled directly below */
            seq_t binom2 = 1;             /* C(m-1,0) */
            for (idx_t k = 1; k <= m - 1; ++k) {
                /* term = l[k] * a[m-k] * binom2 */
                seq_t term = L[k];
                term *= A[m - k];
                term *= binom2;
                sum_l += term;
                /* update binomial for next k */
                if (k < m - 1) {
                    /* binom2 = C(m-1,k) from C(m-1,k-1) */
                    binom2 = binom2 * (seq_t)(m - k) / (seq_t)k;
                }
            }
        }
        L[m] = A[m] - sum_l;             /* for m = 1, sum_l = 0 automatically */
    }
 
    max_computed = (idx_t)n;
    return A[n];
}
