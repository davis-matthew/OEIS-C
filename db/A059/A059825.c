/*
 * A059825  Expansion of series related to Liouville's Last Theorem: g.f.
 *          sum_{t>0} (-1)^(t+1) *x^(t*(t+1)/2) / ( (1-x^t)^8
 *          *product_{i=1..t} (1-x^i) ).
 * https://oeis.org/A059825
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 11, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  OEIS A059825
 *  a(n) = coefficient of x^n in
 *  Σ_{t≥1} (-1)^(t+1)  x^{t(t+1)/2}
 *      / ( (1-x^t)^8 * ∏_{i=1..t}(1-x^i) )
 *
 *  Computation:  use the fact that
 *
 *      1/(1-x^t)^8  = Σ_{c≥0} C(c+7,7) x^{t c}
 *      1/∏_{i=1..t-1}(1-x^i) = Σ_{m≥0} p_{t-1}(m) x^m,
 *      where p_k(m) is the number of partitions of m into parts ≤ k.
 *
 *  Hence
 *
 *      a(n) = Σ_{t≥1} (-1)^(t+1)
 *                 Σ_{c≥0} C(c+7,7) * p_{t-1}(n - t(t+1)/2 - t c)
 *
 *  We accumulate the sums modulo the unsigned type seq_t.
 *  For each t we maintain the array P[m] = p_{t-1}(m).
 *  P is updated incrementally for increasing t.
 *
 *  The algorithm runs in O(N^2) time with a small constant and
 *  uses O(N) memory, easily fitting the limits for N≤10000.
 */

#include <stdlib.h>
#include <stdbool.h>

/* Cache of computed values.  Cached up to the largest n requested. */
static seq_t *cached_vals = NULL;
static size_t cached_size = 0;

/* Return the n‑th term of A059825. */
seq_t a(idx_t n)
{
    /* Size needed for this request. */
    size_t needed = (size_t)n + 1;
    if (n >= cached_size) {
        /* Allocate new arrays for the larger request. */
        seq_t *new_cache = (seq_t *)malloc(needed * sizeof(seq_t));
        if (!new_cache) abort();

        /* Working arrays for the computation up to 'needed-1'. */
        seq_t *P    = (seq_t *)calloc(needed, sizeof(seq_t));   /* partitions up to t‑1 */
        seq_t *pos  = (seq_t *)calloc(needed, sizeof(seq_t));   /* positive contributions */
        seq_t *neg  = (seq_t *)calloc(needed, sizeof(seq_t));   /* negative contributions */
        if (!P || !pos || !neg) abort();

        /* Initial partitions: only part 0 is possible. */
        P[0] = (seq_t)1;

        /* Main loop over t.  Stop when t(t+1)/2 > n. */
        for (idx_t t = 1; ; ++t) {
            /* Triangular number for this t. */
            idx_t tri = (t * (t + 1)) / 2;
            if ((idx_t)(tri) > n) break;

            /* Sign of the term: (-1)^(t+1). */
            int sign = (t % 2 ? 1 : -1);

            /* Maximum c for which tri + t*c ≤ n. */
            size_t max_c = (n >= tri) ? (size_t)((n - tri) / t) : 0;

            /* Store binomial coefficients C(c+7,7) for c=0..max_c. */
            seq_t *comb = (seq_t *)malloc((max_c + 1) * sizeof(seq_t));
            if (!comb) abort();
            comb[0] = (seq_t)1;
            for (size_t c = 1; c <= max_c; ++c) {
                /* comb[c] = comb[c-1] * (c+7+1) / c.  (c+8)/c */
                comb[c] = (comb[c - 1] * (seq_t)(c + 8)) / (seq_t)c;
            }

            /* For each residue r mod t, convolve comb(c) * P[d] along that stride. */
            for (int r = 0; r < t; ++r) {
                if ((size_t)r > (size_t)(n - tri)) continue;
                size_t L = ((size_t)(n - tri) - (size_t)r) / (size_t)t + 1;
                /* For each position q on this residue line. */
                for (size_t q = 0; q < L; ++q) {
                    seq_t sum = 0;
                    size_t max_cq = (q <= max_c) ? q : max_c;
                    for (size_t c = 0; c <= max_cq; ++c) {
                        size_t d = r + t * (q - c);
                        sum += comb[c] * P[d];
                    }
                    idx_t idx = (idx_t)(tri + r + t * q);
                    if (sign > 0)
                        pos[idx] += sum;
                    else
                        neg[idx] += sum;
                }
            }

            free(comb);

            /* Update P to include parts of size t for the next iteration. */
            for (idx_t w = t; w <= n; ++w)
                P[w] += P[w - t];
        }

        /* Build the final values from pos and neg. */
        for (size_t i = 0; i < needed; ++i)
            new_cache[i] = pos[i] - neg[i];   /* final value is non‑negative */

        /* Clean up. */
        free(P);
        free(pos);
        free(neg);
        /* Replace the old cache. */
        free(cached_vals);
        cached_vals = new_cache;
        cached_size = needed;
    }
    return cached_vals[(size_t)n];
}
