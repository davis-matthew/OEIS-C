/*
 * A008405  n-th derivative of x^(1/x) at x=1.
 * https://oeis.org/A008405
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   not stated
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 20, i128 n <= 33,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A008405: n‑th derivative of x^(1/x) at x=1
   Recurrence derived from the exponential of g(x)=lnx/x:
      a(0) = 1
      a(n) = Σ_{k=1..n} C(n-1,k-1) * (-1)^{k+1} * k! * H_k * a(n-k)
   where H_k is the k‑th harmonic number.
   The code precomputes the necessary values of g_k = (-1)^{k+1} k! H_k
   incrementally and uses the above convolution to obtain a(n).  */

#include <stdlib.h>

seq_t a(idx_t n) {
    /* Static storage for computed values */
    static seq_t *A = NULL;          /* a(0..max) */
    static seq_t *g = NULL;          /* g_k = (-1)^{k+1} k! H_k, k>=0 */
    static idx_t A_cap = 0, g_cap = 0;
    static idx_t max_computed = 0;    /* largest index that has been computed */
    static seq_t fact = 1;           /* (max_computed)!  */
    static seq_t T   = 0;            /* T_k = k! H_k  */

    /* --- ensure storage is large enough --------------------------------- */
    if (n >= A_cap) {
        idx_t new_cap = (n + 1) * 2;        /* grow at least to 2*(n+1) */
        if (A_cap == 0)
            A = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            A = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
        for (idx_t i = A_cap; i < new_cap; ++i) A[i] = 0;
        A_cap = new_cap;

        if (g_cap == 0)
            g = (seq_t *)malloc(new_cap * sizeof(seq_t));
        else
            g = (seq_t *)realloc(g, new_cap * sizeof(seq_t));
        for (idx_t i = g_cap; i < new_cap; ++i) g[i] = 0;
        g_cap = new_cap;
    }

    /* --- initialise first element ------------------------------------- */
    if (max_computed == 0) {        /* first call or n==0 after reset */
        A[0] = 1;                   /* a(0) = 1 */
        g[0] = 0;                    /* unused */
        /* fact and T already correspond to k=0: fact=0! = 1, T=0 */
    }

    /* --- compute missing g_k values up to n ---------------------------- */
    if (max_computed < n) {
        for (idx_t k = max_computed + 1; k <= n; ++k) {
            /* update factorial: fact = k! */
            fact *= (seq_t)k;

            /* old T is T_{k-1} */
            seq_t oldT = T;

            /* term = (k-1)! = fact / k   (exact division) */
            seq_t term = fact / (seq_t)k;

            /* new T = k * T_{k-1} + (k-1)!  = k!*H_k */
            T = (seq_t)k * oldT + term;

            /* g_k = (-1)^{k+1} * T */
            seq_t sign = ((k % 2 == 0) ? -((seq_t)1) : ((seq_t)1));
            g[k] = sign * T;
        }
    }

    /* --- compute a(i) for all i from max_computed+1 up to n ----------- */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        seq_t acc = 0;
        seq_t binom = 1;                               /* C(i-1,0) */
        for (idx_t k = 1; k <= i; ++k) {
            /* add term: C(i-1,k-1) * g_k * a[i-k] */
            acc += binom * g[k] * A[i - k];
            /* update binomial coefficient for next k */
            if (k < i) {
                binom = (binom * (seq_t)(i - k)) / (seq_t)k;
            }
        }
        A[i] = acc;
    }
    max_computed = n;

    return A[n];
}
