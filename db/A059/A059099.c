/*
 * A059099  Expansion of e.g.f. exp(exp(x)-1)/(2-exp(x)).
 * https://oeis.org/A059099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = B(n) + sum_{k=0}^{n-1} binom(n,k) * a(k) where B(n) is
 *            the n‑th Bell number B(0)=1, B(n) = sum_{k=0}^{n-1} binom(n-1,k)
 *            * B(k) (n>0) a(0)=1. The recurrence follows from
 *            (2‑exp(x))·G(x)=exp(exp(x)‑1)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static void ensure_capacity(size_t needed, size_t *cap, seq_t **ptr) {
    if (needed <= *cap) return;
    size_t new_cap = *cap;
    if (new_cap == 0) new_cap = 4;
    while (new_cap < needed) new_cap <<= 1;
    *ptr = (seq_t *)realloc(*ptr, new_cap * sizeof(seq_t));
    *cap = new_cap;
}

seq_t a(idx_t n)
{
    /* static storage for a(n) and B(n) */
    static seq_t *A = NULL;       /* a values */
    static seq_t *B = NULL;       /* Bell numbers */
    static size_t A_cap = 0, B_cap = 0, A_len = 0;

    size_t N = (size_t)n;         /* non‑negative index assumption */
    if (N == 0) return (seq_t)1;  /* a(0) = 1 */

    /* initialise empty arrays */
    if (A_len == 0) {
        A_cap = 4;
        B_cap = 4;
        A = (seq_t *)malloc(A_cap * sizeof(seq_t));
        B = (seq_t *)malloc(B_cap * sizeof(seq_t));
        A[0] = (seq_t)1;   /* a(0) */
        B[0] = (seq_t)1;   /* B(0) */
        A_len = 1;
    }

    /* extend arrays up to N if necessary */
    ensure_capacity(N + 1, &A_cap, &A);
    ensure_capacity(N + 1, &B_cap, &B);

    /* compute values for indices A_len .. N */
    for (size_t i = A_len; i <= N; ++i) {
        /* --- compute Bell number B[i] --- */
        seq_t binomB = (seq_t)1;  /* binom(i-1,0) */
        seq_t sumB   = (seq_t)0;
        for (size_t k = 0; k < i; ++k) {
            sumB += binomB * B[k];
            /* update binomB to binom(i-1,k+1) */
            binomB = binomB * (seq_t)(i - 1 - k) / (seq_t)(k + 1);
        }
        B[i] = sumB;

        /* --- compute a[i] --- */
        seq_t binomA = (seq_t)1;  /* binom(i,0) */
        seq_t sumA   = (seq_t)0;
        for (size_t k = 0; k < i; ++k) {
            sumA += binomA * A[k];
            /* update binomA to binom(i,k+1) */
            binomA = binomA * (seq_t)(i - k) / (seq_t)(k + 1);
        }
        A[i] = sumA + B[i];   /* a(i) = B(i) + sum_{k<i} binom(i,k) a(k) */
    }

    A_len = N + 1;  /* all computed up to N */
    return A[N];
}
