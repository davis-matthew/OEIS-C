/*
 * A175361  Partial sums of A000141.
 * https://oeis.org/A175361
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 22, u32 n <= 939, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * A175361 – Partial sums of r_6(k), where r_6(k) is the number of
 * representations of k as a sum of 6 squares of integers.
 * Implementation by dynamic programming: compute r_6(k) for
 * k = 0 … n by successive convolutions with the square sequence,
 * then accumulate the partial sums.
 * Complexity:  O(6 * n * sqrt(n))  →  O(n * sqrt(n)),
 * well below the 2‑second limit for n ≈ 10 000.
 */
#include <stdlib.h>
#include <stddef.h>   /* size_t, NULL */

#ifdef __cplusplus
#include <algorithm> /* std::fill */
#endif

/* ------------------------------------------------------------------
 *  a(n) = sum_{k=0}^{n} r_6(k)
 *  where r_6(k) is obtained by exp(6·log(θ_3(x))) where
 *  θ_3(x) = Σ_{m∈ℤ} x^{m^2}.
 *  The convolution is performed 6 times, once for each square,
 *  accounting for both positive and negative m (weight 2 except for 0).
 * ------------------------------------------------------------------ */

seq_t a(idx_t n)
{
    /* allocate auxiliary arrays for DP */
    idx_t i, m, d;
    seq_t *prev = NULL;
    seq_t *curr = NULL;

#if defined(__cplusplus)
    /* C++: use new[] and zero‑initialize */
    prev = new seq_t[n + 1];
    curr = new seq_t[n + 1];
    std::fill(prev, prev + n + 1, (seq_t)0);
    std::fill(curr, curr + n + 1, (seq_t)0);
#else
    /* C: use calloc to obtain zero‑initialized memory */
    prev = (seq_t *)calloc(n + 1, sizeof(seq_t));
    curr = (seq_t *)calloc(n + 1, sizeof(seq_t));
#endif
    if (!prev || !curr) {
#ifdef __cplusplus
        delete[] prev;
        delete[] curr;
#else
        free(prev);
        free(curr);
#endif
        return (seq_t)0;  /* allocation failure */
    }

    /* initial state: one way to build 0 using 0 squares  */
    prev[0] = (seq_t)1;

    /* pre‑compute all squares ≤ n */
    idx_t maxm = 0;
    while ((maxm + 1) * (maxm + 1) <= n) maxm++;

    /* allocate array of squares */
    idx_t *sq = NULL;
#if defined(__cplusplus)
    sq = new idx_t[maxm + 1];
    for (m = 0; m <= maxm; ++m) sq[m] = m * m;
#else
    sq = (idx_t *)malloc((maxm + 1) * sizeof(idx_t));
    for (m = 0; m <= maxm; ++m) sq[m] = m * m;
#endif

    /* perform 6 successive convolutions (6 variables) */
    for (i = 1; i <= 6; ++i) {
        /* zero destination array */
        for (m = 0; m <= n; ++m) curr[m] = (seq_t)0;

        /* convolution with square contributions */
        for (m = 0; m <= maxm; ++m) {
            d = sq[m];
            seq_t weight = (m == 0) ? (seq_t)1 : (seq_t)2;
            for (idx_t t = 0; t + d <= n; ++t) {
                if (prev[t] == (seq_t)0) continue;
                curr[t + d] += prev[t] * weight;
            }
        }

        /* swap buffers for next iteration */
        seq_t *tmp = prev; prev = curr; curr = tmp;
    }

    /* accumulate partial sums */
    seq_t running = (seq_t)0;
    for (i = 0; i <= n; ++i) {
        running += prev[i];
        prev[i] = running;
    }

    seq_t result = prev[n];

    /* free resources */
#if defined(__cplusplus)
    delete[] prev;
    delete[] curr;
    delete[] sq;
#else
    free(prev);
    free(curr);
    free(sq);
#endif

    return result;
}
