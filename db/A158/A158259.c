/*
 * A158259  L.g.f.: exp(Sum_{n>=1} a(n)*x^n/n) = 1 + x*exp(Sum_{n>=1}
 *          C(2n-1,n)*a(n)*x^n/n) where C(2n-1,n) = A001700(n-1).
 * https://oeis.org/A158259
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Formula: L.g.f.: exp( Sum_{n>=1} a(n)*x^n/n ) = 1 + x*exp( Sum_{n>=1} C(2n-1,n)*a(n)*x^n/n )
 *  where C(2n-1,n) = binomial(2n-1,n).  This function computes the OEIS sequence A158259.
 *  The sequence is defined for n>=0 with a(0)=1; the recurrence below is derived
 *  from the functional equation and is implemented with integer arithmetic only.
 */

#include <stdlib.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Handle the offset term a(0)=1 */
    if (n == 0)
        return (seq_t)1;            /* a(0) = 1 */

    /* Arrays for b(k) = a(k) for k>=1 (used in the recurrence), 
       P(k) = k*F_k where F_k is the k-th coefficient of exp(f),
       G(k) = coefficient of exp(g), and C(k) = binomial(2k-1,k). */
    static seq_t *b   = NULL;   /* indices 1 .. max_n */
    static seq_t *P   = NULL;   /* indices 1 .. max_n */
    static seq_t *G   = NULL;   /* indices 0 .. max_n */
    static seq_t *C   = NULL;   /* indices 1 .. max_n */
    static idx_t  max_n = 0;    /* highest n for which the arrays are filled */

    /* We need values up to k = n+1, because OEIS a(n) = b(n+1) for n>=1. */
    idx_t target = n + 1;   /* maximum k we must compute */

    /* Allocate or enlarge arrays if necessary. */
    if (max_n < target) {
        idx_t new_size = target + 1;          /* include index 0 for G */
        if (!b) {
            b = (seq_t*)malloc(new_size * sizeof(seq_t));
            P = (seq_t*)malloc(new_size * sizeof(seq_t));
            G = (seq_t*)malloc(new_size * sizeof(seq_t));
            C = (seq_t*)malloc(new_size * sizeof(seq_t));
        } else {
            b = (seq_t*)realloc(b, new_size * sizeof(seq_t));
            P = (seq_t*)realloc(P, new_size * sizeof(seq_t));
            G = (seq_t*)realloc(G, new_size * sizeof(seq_t));
            C = (seq_t*)realloc(C, new_size * sizeof(seq_t));
        }
        /* Initialize new entries to zero. */
        for (idx_t i = max_n + 1; i <= target; ++i) {
            b[i] = 0;
            P[i] = 0;
            G[i] = 0;
            C[i] = 0;
        }
        G[0] = (seq_t)1;        /* G_0 = 1 for all computations */

        /* Pre‑compute binomial coefficients C(k) = binomial(2k-1,k) for new indices. */
        for (idx_t i = max_n + 1; i <= target; ++i)
            C[i] = binom((idx_t)(2 * i - 1), i);

        max_n = target;
    }

    /* Compute terms for k = max_n_prev+1 .. target if they are not already computed. */
    static idx_t computed = 0;          /* largest k already computed */
    for (idx_t k = computed + 1; k <= target; ++k) {
        /* 1. Compute sumP_k = sum_{j=1}^{k-1} (b[j] * P[k-j]) / (k-j)  */
        seq_t sumP = 0;
        for (idx_t j = 1; j < k; ++j) {
            seq_t prod = b[j] * P[k - j];
            prod /= (k - j);          /* division is exact by construction */
            sumP += prod;
        }

        /* 2. Compute b[k] = k * G[k-1] - sumP   (integer arithmetic only) */
        seq_t bk = ((seq_t)k) * G[k - 1] - sumP;
        b[k] = bk;

        /* 3. Compute P[k] = b[k] + sumP   (this is k * F_k) */
        P[k] = bk + sumP;

        /* 4. Compute sumG_k = sum_{j=1}^{k-1} C[j] * b[j] * G[k-j]   */
        seq_t sumG = 0;
        for (idx_t j = 1; j < k; ++j) {
            seq_t temp = C[j] * b[j];
            temp *= G[k - j];
            sumG += temp;
        }

        /* 5. Compute G[k] = ( C[k] * b[k] + sumG ) / k */
        seq_t numerator = C[k] * bk + sumG;
        G[k] = numerator / k;
    }
    computed = target;   /* all needed terms are now computed */

    /* OEIS term a(n) corresponds to b[n+1] for n >= 1. */
    return b[n + 1];
}
