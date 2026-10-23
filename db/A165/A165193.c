/*
 * A165193  Eigensequence of triangle A038208.
 * https://oeis.org/A165193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 8, u64 n <= 11, u128 n <= 16,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A165193 - Eigensequence of triangle A038208.
 * Derivation:
 *   From the functional equation
 *      A(x) = 1 + x*A(2x/(1-2x))/(1-2x),
 *   equating coefficients leads to the recurrence
 *      a(0) = 1, a(1) = 1
 *      a(n) = 2*a(n-1) + 2^(n-1) * Σ_{k=0}^{n-2} a(k+1) * C(n-2, k)   for n ≥ 2
 *   where C denotes the binomial coefficient.
 *
 * This implementation uses the recurrence directly.
 */

#include <stdlib.h>

#ifdef __cplusplus
#include <vector>
#else
/* For C builds, seq_t is assumed to be a POD integer type,
 * so raw memory allocation suffices. */
#endif

seq_t a(idx_t n)
{
    /* base case */
    if (n == 0) return (seq_t)1;
    /* dynamic storage for sequence values */
#ifdef __cplusplus
    static std::vector<seq_t> seq;
    if (seq.size() <= n) seq.resize(n + 1);
#else
    static seq_t *seq = NULL;
    static idx_t capacity = 0;
    if (n + 1 > capacity) {
        size_t newcap = (n + 1) * 2;
        seq_t *tmp = (seq_t *)realloc(seq, newcap * sizeof(seq_t));
        if (!tmp) return (seq_t)0; /* allocation failure */
        seq = tmp;
        capacity = newcap;
    }
#endif

    /* initialise known terms */
    seq[0] = (seq_t)1;
    if (n >= 1) seq[1] = (seq_t)1;

    /* pre‑compute powers of two: pow2[i] = 2^i */
    seq_t pow2_prev = (seq_t)1; /* 2^0 */

    /* compute a(i) for i = 2 … n */
    for (idx_t i = 2; i <= n; ++i) {
        /* 2^(i-1) */
        seq_t pow2 = pow2_prev * (seq_t)2;
        /* Σ_{k=0}^{i-2} a(k+1) * C(i-2, k) */
        seq_t sum = (seq_t)0;
        seq_t bin = (seq_t)1;            /* C(i-2, 0) */
        idx_t m = i - 2;                 /* upper index of binomial */
        for (idx_t k = 0; k <= m; ++k) {
            sum += seq[k + 1] * bin;
            if (k < m) {
                /* recurrence for binomial:
                 * C(n, k+1) = C(n, k) * (n-k)/(k+1)   (exact division)      */
                seq_t numer = bin * (seq_t)(m - k);
                seq_t denom = (seq_t)(k + 1);
                bin = numer / denom;
            }
        }
        seq[i] = seq[i - 1] * (seq_t)2 + pow2 * sum;
        /* advance power of two for next iteration */
        pow2_prev = pow2;
    }

    return seq[n];
}
