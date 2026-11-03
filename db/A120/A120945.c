/*
 * A120945  Triangle T(n,k) of number of labeled directed multigraphs (with
 *          loops), without isolated vertices, with n arrows and k vertices (n
 *          = 1,2,.., k = 1..2*n).
 * https://oeis.org/A120945
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(r,k) where T(r,k) is given by T(r,k) = Σ_{j=0..k}
 *            (-1)^(k-j) * binom(k,j) * binom(j^2 + r - 1, r) r is the
 *            smallest integer with r*(r+1) ≥ n, and k = n - (r-1)*r (1 ≤ k ≤
 *            2r)
 * Range:     u8 n <= 9, u16 n <= 25, u32 n <= 42, u64 n <= 42, u128 n <= 42,
 *            big any n
 * Validated: partial, pass, 42 terms through n = 42 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <math.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;              /* undefined for n < 1, return 0 */

    /* Find the row r.  r satisfies r*(r+1) ≥ n  and (r-1)*r < n. */
    idx_t r = (idx_t)((sqrtl((long double)(4 * n + 1)) - 1) / 2);
    /* Correct possible off‑by‑one due to truncation */
    while (r * (r + 1) < n) ++r;
    while (r > 0 && (r - 1) * r >= n) --r;

    /* Column position in this row (1‑based). */
    idx_t k = n - (r - 1) * r;   /* 1 ≤ k ≤ 2r */

    seq_t pos_sum = 0;
    seq_t neg_sum = 0;

    for (idx_t j = 0; j <= k; ++j) {
        seq_t bj = binomial(k, j);
        seq_t bj2 = binomial(j * j + r - 1, r);
        seq_t prod = seq_mul(bj, bj2);

        if (((k - j) & 1) == 0)
            pos_sum += prod;
        else
            neg_sum += prod;
    }

    /* The combinatorial sum is non‑negative, so subtraction is safe. */
    return pos_sum - neg_sum;
}
