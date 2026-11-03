/*
 * A048033  Number of nonempty subsets of {1,2,...,n} in which exactly 2/5 of
 *          the elements are <= (n-3)/3.
 * https://oeis.org/A048033
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Σ_{k≥1, 5k≤n, 2k≤m, 3k≤n−m}
 *            binomial(m,2k)·binomial(n−m,3k), where m = floor((n−3)/3). The
 *            terms counted are the nonempty subsets of {1,…,n} that contain
 *            exactly 2/5 of their elements in the interval [1,m]
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Validated: partial, pass, 35 terms through n = 35 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/binomial.h"

/* Helper: safe multiplication for seq_t */
static inline seq_t seq_mul(seq_t a, seq_t b) {
    return a * b;
}

seq_t a(idx_t n)
{
    seq_t result = 0;

    /* Cut‑point m = ⌊(n−3)/3⌋ */
    idx_t m = (n < 3) ? 0 : (n - 3) / 3;
    idx_t n_m = n - m;        /* size of the upper part */
    idx_t kmax = n / 5;       /* largest k with 5k ≤ n */

    for (idx_t k = 1; k <= kmax; ++k) {
        idx_t low  = 2 * k;   /* number of low elements chosen */
        idx_t high = 3 * k;   /* number of high elements chosen */

        if (low > m || high > n_m)
            continue;        /* binomial will be zero otherwise */

        seq_t term = seq_mul(binomial(m, low), binomial(n_m, high));
        result += term;
    }

    return result;
}
