/*
 * A229331  Total sum of 9th powers of parts in all partitions of n.
 * https://oeis.org/A229331
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{t=1..n} sigma_9(t) * p(n-t) where
 *            sigma_9(t)=Sum_{k|t} k^9, p(m) is the partition function
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 10, u64 n <= 66, u128 n <= 481,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  The implementation precomputes p[0..n], sigma_9[0..n] and their
    convolution a[0..n] each time a larger n is requested.  All
    computations use only integer operations on seq_t. */
#include <stdlib.h>
#include <stdint.h>
#ifdef __cplusplus
#include <algorithm>
#endif

/* static data */
static seq_t *P      = NULL;  /* partition numbers p(n) */
static seq_t *Sigma  = NULL;  /* sigma_9(n)   */
static seq_t *A      = NULL;  /* result a(n)  */
static idx_t  max_computed = 0;   /* largest n for which A[n] is valid */
static idx_t  capacity     = 0;   /* currently allocated array size */

/* Ensure all arrays have space for indices up to 'needed' (inclusive). */
static void ensure_capacity(idx_t needed) {
    if (capacity > needed) return;

    idx_t new_cap = needed + 1;   /* include index 0 */

    seq_t *new_P, *new_Sigma, *new_A;
#ifdef __cplusplus
    new_P     = new seq_t[new_cap];
    new_Sigma = new seq_t[new_cap];
    new_A     = new seq_t[new_cap];
    if (P)     { std::copy    (P,       P   + capacity, new_P);     delete[] P; }
    if (Sigma) { std::copy    (Sigma,   Sigma + capacity, new_Sigma);delete[] Sigma; }
    if (A)     { std::copy    (A,       A   + capacity, new_A);     delete[] A; }
#else
    new_P     = (seq_t*)realloc(P,     new_cap * sizeof(seq_t));
    new_Sigma = (seq_t*)realloc(Sigma,new_cap * sizeof(seq_t));
    new_A     = (seq_t*)realloc(A,     new_cap * sizeof(seq_t));
    if (!new_P || !new_Sigma || !new_A) abort();
#endif

    /* Zero-initialize newly allocated entries */
    for (idx_t i = capacity; i < new_cap; ++i) {
        new_P[i]     = 0;
        new_Sigma[i] = 0;
        new_A[i]     = 0;
    }

    P      = new_P;
    Sigma  = new_Sigma;
    A      = new_A;
    capacity = new_cap;
}

/* Recompute all auxiliary tables up to N (inclusive). */
static void compute_up_to(idx_t N) {
    if (N <= max_computed) return;

    ensure_capacity(N);

    /* reset arrays */
    for (idx_t i = 0; i <= N; ++i) {
        P[i]      = 0;
        Sigma[i]  = 0;
        A[i]      = 0;
    }
    P[0] = 1;                     /* p(0) = 1 */

    /* partition numbers: p[n] = sum_{k=1..n} p[n-k] for each part size k */
    for (idx_t k = 1; k <= N; ++k) {
        for (idx_t m = k; m <= N; ++m) {
            P[m] += P[m - k];
        }
    }

    /* sigma_9: sigma_9(t) = sum_{k|t} k^9 */
    for (idx_t k = 1; k <= N; ++k) {
        seq_t k9 = 1;
        seq_t k_seq = (seq_t)k;
        for (int e = 0; e < 9; ++e) k9 *= k_seq;

        for (idx_t m = k; m <= N; m += k) {
            Sigma[m] += k9;
        }
    }

    /* convolution: a[n] = sum_{t=1..n} sigma_9(t) * p[n-t] */
    for (idx_t t = 1; t <= N; ++t) {
        seq_t sigma_t = Sigma[t];
        for (idx_t m = t; m <= N; ++m) {
            seq_t prod = sigma_t;
            prod *= P[m - t];
            A[m] += prod;
        }
    }

    max_computed = N;
}

/* Public function required by the build system */
seq_t a(idx_t n) {
    if (n == 0) return 0;
    compute_up_to(n);
    return A[n];
}
