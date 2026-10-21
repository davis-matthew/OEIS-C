/*
 * A240487  Number of partitions of n containing m(2) as a part, where m
 *          denotes multiplicity.
 * https://oeis.org/A240487
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 19, u16 n <= 47, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = number of partitions of n having m copies of part 2 and
 * also containing a part of size m (m ≥ 1).  This implements the
 * definition of OEIS A240487. */

#include <stdlib.h>

/* Computes a(n) with static memoisation of the helper arrays. */
seq_t a(idx_t n) {
    /* Memoised partitions that exclude part 2.  dp_excl2[s] = number of
     * partitions of s that use parts ≠ 2.
     */
    static seq_t *dp_excl2 = NULL;
    static idx_t dp_size = 0;

    /* Allocate / recompute dp_excl2 up to n if necessary. */
    if (n + 1 > dp_size) {
        idx_t new_size = n + 1;
        seq_t *new_dp = (seq_t*)calloc(new_size, sizeof(seq_t));
        if (!new_dp) return 0;          /* allocation failed */
        new_dp[0] = 1;                  /* empty partition */

        for (idx_t part = 1; part <= n; ++part) {
            if (part == 2) continue;    /* exclude part 2 */
            for (idx_t sum = part; sum <= n; ++sum) {
                new_dp[sum] += new_dp[sum - part];
            }
        }

        if (dp_excl2) free(dp_excl2);
        dp_excl2 = new_dp;
        dp_size = new_size;
    }

    seq_t total = 0;
    idx_t max_m = n / 2;                 /* max possible multiplicity of 2 */

    for (idx_t m = 1; m <= max_m; ++m) {
        if (m == 1) {
            /* Exactly one 2 and at least one part 1.  Remove 2+1 = 3 from n. */
            if (n >= 3) total += dp_excl2[n - 3];
        } else if (m == 2) {
            /* Exactly two 2's; remaining sum must exclude more 2's. */
            if (n >= 4) total += dp_excl2[n - 4];
        } else {
            /* m ≥ 3: sum over k≥1 copies of part m.
             * t = k + 2 (≥3).  For each t, subtract copies of 2 and m
             * from the remainder.
             */
            idx_t tmax = n / m;
            for (idx_t t = 3; t <= tmax; ++t) {
                idx_t rem = n - m * t;
                if (rem > n) continue;         /* safety */
                if (rem < 0) break;            /* further t will be even smaller */

                seq_t val = dp_excl2[rem];
                if (rem >= m) val -= dp_excl2[rem - m]; /* exclude partitions containing part m */
                total += val;
            }
        }
    }
    return total;
}
