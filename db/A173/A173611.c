/*
 * A173611  Self-convolution of A173610.
 * https://oeis.org/A173611
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 22, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Self-convolution of A173610.
   Formula: a(n) = Σ_{k=0..n} f(k)·f(n‑k) where
     f(0)   = 1
     f(2m)  = a(m‑1)·a(m)   (m≥1)
     f(2m+1)= a(m)²
   This is equivalent to the generating‑function description
   A(x) = (C(x²)+x·B(x²))² with
     B(x)=∑a(n)²xⁿ   and   C(x)=1+∑a(n)a(n+1)xⁿ⁺¹.
   The implementation uses memoisation so that calls may be made in
   any order.  It runs in O(n²) time and O(n) space, which is
   comfortably fast for n up to about 10 000. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static memoisation tables */
    static seq_t *A = NULL;       /* a-values */
    static seq_t *S = NULL;       /* f-values (s in OEIS terminology) */
    static idx_t  max_computed = 0; /* highest n already computed */
    static idx_t  capacity     = 0; /* allocated size of tables */

    /* ------------------------------------------------------------------
       Allocate / grow tables to accommodate index n.
       ------------------------------------------------------------------ */
    if (n >= capacity) {
        idx_t new_cap = capacity ? capacity << 1 : 1;
        while (new_cap <= n) new_cap <<= 1;
        A = (seq_t*)realloc(A, new_cap * sizeof *A);
        S = (seq_t*)realloc(S, new_cap * sizeof *S);
        capacity = new_cap;
    }

    /* ------------------------------------------------------------------
       Initialise base case for the first call.
       ------------------------------------------------------------------ */
    if (max_computed == 0) {
        A[0] = (seq_t)1;
        S[0] = (seq_t)1;
        max_computed = 0;
    }

    /* ------------------------------------------------------------------
       Compute values from max_computed+1 up to n.
       ------------------------------------------------------------------ */
    for (idx_t i = max_computed + 1; i <= n; ++i) {
        /* Compute f(i) from already known a-values */
        if ((i & 1) == 0) {          /* even: i = 2m */
            idx_t m = i >> 1;
            if (m > 0)
                S[i] = A[m - 1] * A[m];
            else
                S[i] = (seq_t)0;     /* this case never occurs for i>=1 */
        } else {                      /* odd: i = 2m+1 */
            idx_t m = (i - 1) >> 1;
            S[i] = A[m] * A[m];
        }

        /* Compute a(i) = Σ_{k=0..i} S[k]·S[i‑k] */
        seq_t sum = (seq_t)0;
        for (idx_t k = 0; k <= i; ++k) {
            sum += S[k] * S[i - k];
        }
        A[i] = sum;
    }

    /* Update memoisation bound */
    max_computed = n;
    return A[n];
}
