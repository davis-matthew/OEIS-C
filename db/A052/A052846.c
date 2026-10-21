/*
 * A052846  E.g.f.: (1-exp(x))*log(2-exp(x)).
 * https://oeis.org/A052846
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 5, u16 n <= 7, u32 n <= 12, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence for sequence A052846 (OEIS): 
   B(0) = 1,  B(n) = 1 + Σ_{k=0}^{n-1} C(n,k)·B(k)    for n≥1
   a(0) = 0, a(1) = 0, a(n) = a(n-1) + B(n-2) + B(n-1) – 1   for n≥2
   (B and a are indexed by n).  The implementation memoizes both
   sequences in static dynamic arrays. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* ----- B sequence ----- */
    static seq_t *B = NULL;         /* Holds B(0),B(1),...             */
    static size_t B_cap = 0;        /* Allocated size of B             */
    static idx_t B_max = 0;         /* Largest index computed (>=0)    */

    /* Make sure B[0] exists */
    if (B == NULL) {
        B_cap = 1;
        B = (seq_t *)malloc(B_cap * sizeof(seq_t));
        if (B == NULL) { return (seq_t)0; }           /* OOM fallback */
        B[0] = (seq_t)1;
        B_max = 0;
    }

    /* Extend B up to index max(n-1,0) */
    idx_t need_B = (n >= 1) ? n - 1 : 0;
    if ((size_t)need_B >= B_cap) {
        size_t new_cap = need_B + 1;
        seq_t *tmp = (seq_t *)realloc(B, new_cap * sizeof(seq_t));
        if (tmp == NULL) return (seq_t)0;           /* OOM fallback */
        B = tmp; B_cap = new_cap;
    }
    for (idx_t cur = B_max + 1; cur <= need_B; ++cur) {
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;            /* C(cur,0) */
        for (idx_t k = 0; k < cur; ++k) {
            sum += bin * B[k];
            /* Next binomial: C(cur,k+1) = C(cur,k) * (cur-k)/(k+1) */
            bin = bin * (cur - k) / (k + 1);
        }
        B[cur] = (seq_t)1 + sum;
    }
    B_max = need_B;

    /* ----- a sequence ----- */
    static seq_t *A = NULL;         /* Holds a(0),a(1),...             */
    static size_t A_cap = 0;        /* Allocated size of A             */
    static idx_t A_max = 0;         /* Largest index computed (>=0)    */

    /* Make sure A[0] exists and set initial values */
    if (A == NULL) {
        A_cap = 1;
        A = (seq_t *)malloc(A_cap * sizeof(seq_t));
        if (A == NULL) { return (seq_t)0; }           /* OOM fallback */
        A[0] = (seq_t)0;
        if (n >= 1) {
            A_cap = 2;
            A = (seq_t *)realloc(A, A_cap * sizeof(seq_t));
            A[1] = (seq_t)0;
        }
        A_max = (n >= 1) ? 1 : 0;
    } else {
        /* Ensure capacity for n */
        if ((size_t)n + 1 > A_cap) {
            size_t new_cap = (size_t)n + 1;
            seq_t *tmp = (seq_t *)realloc(A, new_cap * sizeof(seq_t));
            if (tmp == NULL) return (seq_t)0;          /* OOM fallback */
            A = tmp; A_cap = new_cap;
        }
        /* If the array was resized to allow index 1, initialise it */
        if (A_cap > 1 && A_max < 1) {
            A[1] = (seq_t)0;
        }
        if (A_max < 0) A_max = 0; /* should never happen */
    }

    /* Compute a values up to n */
    for (idx_t cur = A_max + 1; cur <= n; ++cur) {
        if (cur == 0) { A[0] = (seq_t)0; }
        else if (cur == 1) { A[1] = (seq_t)0; }
        else {
            A[cur] = A[cur - 1] + B[cur - 2] + B[cur - 1] - (seq_t)1;
        }
    }
    A_max = n;

    return A[n];
}
