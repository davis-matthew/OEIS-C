/*
 * A143342  G.f. satisfies: A(x) = 1 + x*A(x)^5/A(-x).
 * https://oeis.org/A143342
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(0) = a(1) = 1. For n ≥ 2: Let pow5[k] be the coefficient of
 *            x^k in A(x)^5. Then a(n) = pow5[n-1] + Σ_{i=1}^{n-1}
 *            a(i)·a(n-i)·sgn(n-i) where sgn(t) = 1 if t is odd, −1 if t is
 *            even. The values pow5[0],pow5[1],… are built incrementally:
 *            pow2[k] = Σ_{i=0}^{k} a(i)·a(k-i) pow3[k] = Σ_{i=0}^{k} a(i)...
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 19, u128 n <= 19,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n)
{
    /* Static state to memoize computed values across calls */
    static size_t max_computed = 0;          /* highest computed index */
    static seq_t *A  = NULL;                /* a[0..max_computed]   */
    static seq_t *P2 = NULL;                /* a^2                  */
    static seq_t *P3 = NULL;                /* a^3                  */
    static seq_t *P4 = NULL;                /* a^4                  */
    static seq_t *P5 = NULL;                /* a^5                  */
    static size_t allocated = 0;             /* current allocation   */

    /* Allocate or grow arrays if necessary */
    if (n >= allocated) {
        size_t newalloc = (size_t)n + 1;

        /* Reallocate arrays */
        A  = (seq_t *)realloc(A,  newalloc * sizeof(seq_t));
        P2 = (seq_t *)realloc(P2, newalloc * sizeof(seq_t));
        P3 = (seq_t *)realloc(P3, newalloc * sizeof(seq_t));
        P4 = (seq_t *)realloc(P4, newalloc * sizeof(seq_t));
        P5 = (seq_t *)realloc(P5, newalloc * sizeof(seq_t));

        /* Initialize new entries to zero */
        memset(A +  allocated, 0, (newalloc - allocated)*sizeof(seq_t));
        memset(P2 + allocated, 0, (newalloc - allocated)*sizeof(seq_t));
        memset(P3 + allocated, 0, (newalloc - allocated)*sizeof(seq_t));
        memset(P4 + allocated, 0, (newalloc - allocated)*sizeof(seq_t));
        memset(P5 + allocated, 0, (newalloc - allocated)*sizeof(seq_t));

        allocated = newalloc;
    }

    /* Initialization on first call */
    if (max_computed == 0) {
        A[0]  = 1;
        P2[0] = 1;   /* 1*1   */
        P3[0] = 1;   /* 1*1*1 */
        P4[0] = 1;   /* 1^4   */
        P5[0] = 1;   /* 1^5   */
        max_computed = 0;
    }

    /* Compute values up to the requested n */
    for (idx_t idx = max_computed + 1; idx <= n; ++idx)
    {
        /* Compute the convolution terms of a_i*a_{idx-i} with sign */
        seq_t term_even = 0;
        seq_t term_odd  = 0;

        for (idx_t k = 1; k < idx; ++k) {
            seq_t prod = A[k] * A[idx - k];
            if ((idx - k) & 1) {
                term_odd += prod;          /* (idx - k) is odd */
            } else {
                term_even += prod;         /* (idx - k) is even */
            }
        }

        /* a[idx] = pow5[idx-1] + term_odd - term_even */
        seq_t temp = P5[idx - 1] + term_odd;  /* add first */
        A[idx] = temp - term_even;            /* then subtract */

        /* Update powers of the generating function */

        /* pow2[idx] = Σ_{i=0}^{idx} a[i] * a[idx-i] */
        seq_t sum = 0;
        for (idx_t k = 0; k <= idx; ++k)
            sum += A[k] * A[idx - k];
        P2[idx] = sum;

        /* pow3[idx] = Σ_{i=0}^{idx} a[i] * P2[idx-i] */
        sum = 0;
        for (idx_t k = 0; k <= idx; ++k)
            sum += A[k] * P2[idx - k];
        P3[idx] = sum;

        /* pow4[idx] = Σ_{i=0}^{idx} a[i] * P3[idx-i] */
        sum = 0;
        for (idx_t k = 0; k <= idx; ++k)
            sum += A[k] * P3[idx - k];
        P4[idx] = sum;

        /* pow5[idx] = Σ_{i=0}^{idx} a[i] * P4[idx-i] */
        sum = 0;
        for (idx_t k = 0; k <= idx; ++k)
            sum += A[k] * P4[idx - k];
        P5[idx] = sum;
    }

    max_computed = n;
    return A[n];
}
