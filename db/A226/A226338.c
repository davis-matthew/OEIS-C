/*
 * A226338  Number of nondecreasing -6..6 vectors of length n whose dot
 *          product with some other -6..6 vector equals n.
 * https://oeis.org/A226338
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Count of nondecreasing vectors v (length n, entries –6…6) such
 *            that ∃ w (entries –6…6) with Σ v_i w_i = n. The condition is
 *            equivalent to: 1. 6·Σ|v_i| ≥ n, 2. gcd(v) | n. Hence the code
 *            enumerates all such vectors via inclusion–exclusion on the
 *            possible gcd’s (1…6)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 18, u64 n <= 18, u128 n <= 18,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/binom.h"

/* compute F_g : number of nondecreasing vectors of length n
   with all entries multiples of g  (range –6…6)  and 6·Σ|v_i| ≥ n */
static seq_t compute_F(idx_t n, idx_t g, idx_t T) {
    /* integers for internal use */
    idx_t m   = 6 / g;                     /* floor(6/g)  */
    idx_t A   = 2 * m + 1;                 /* number of allowed values */
    seq_t totalC = binom(n + A - 1, A - 1); /* all nondecreasing vectors */

    /* threshold on Σ|k| where v_i = g·k_i */
    idx_t threshold = (T + g - 1) / g;              /* ceil<T/g> */
    seq_t sub_C = 0;                                /* vectors failing the bound */

    if (threshold > 0) {
        size_t thresh = (size_t)threshold;
        size_t len     = (size_t)(n + 1) * thresh;    /* dp size */
        seq_t *dp      = (seq_t*)malloc(len * sizeof(seq_t));
        seq_t *dp_next = (seq_t*)malloc(len * sizeof(seq_t));
        if (!dp || !dp_next)                          /* safe guard */
            return 0;

        memset(dp, 0, len * sizeof(seq_t));
        dp[0] = 1;                                   /* k=0, s=0 */

        for (idx_t w = 1; w <= m; ++w) {             /* weight from 1..m */
            memset(dp_next, 0, len * sizeof(seq_t));
            for (idx_t k = 0; k <= n; ++k) {
                idx_t base = k * thresh;
                for (idx_t s = 0; s < threshold; ++s) {
                    seq_t val = dp[base + s];
                    if (!val) continue;
                    /* maximum t that keeps s + t*w < threshold and k+t ≤ n */
                    idx_t max_t = (threshold - 1 - s) / w;
                    if (k + max_t > n) max_t = n - k;
                    for (idx_t t = 0; t <= max_t; ++t) {
                        idx_t idx2 = (k + t) * thresh + (s + t * w);
                        seq_t factor = (t == 0) ? (seq_t)1 : (seq_t)(t + 1);
                        dp_next[idx2] += val * factor;
                    }
                }
            }
            /* swap dp and dp_next for next weight */
            seq_t *tmp = dp;
            dp = dp_next;
            dp_next = tmp;
        }

        /* sum over all k, s < threshold to count vectors violating the bound */
        for (idx_t k = 0; k <= n; ++k) {
            idx_t base = k * thresh;
            for (idx_t s = 0; s < threshold; ++s) {
                sub_C += dp[base + s];
            }
        }
        free(dp);
        free(dp_next);
    }

    return totalC - sub_C;
}

seq_t a(idx_t n) {
    /* ceil(n/6) for the dot‑product bound */
    idx_t T = (n + 5) / 6;

    seq_t F[7] = {0};   /* index by g (1..6) */
    for (idx_t g = 1; g <= 6; ++g)
        F[g] = compute_F(n, g, T);

    /* Inclusion‑exclusion over gcd’s (6 → 1) */
    seq_t G6 = F[6];
    seq_t G5 = F[5];
    seq_t G4 = F[4];
    seq_t G3 = F[3] - G6;
    seq_t G2 = F[2] - G4 - G6;
    seq_t G1 = F[1] - G2 - G3 - G4 - G5 - G6;

    /* Sum over divisors of n (≤6) – these are the only admissible gcd’s */
    seq_t result = 0;
    if (n % 1 == 0) result += G1;
    if (n % 2 == 0) result += G2;
    if (n % 3 == 0) result += G3;
    if (n % 4 == 0) result += G4;
    if (n % 5 == 0) result += G5;
    if (n % 6 == 0) result += G6;
    return result;
}
