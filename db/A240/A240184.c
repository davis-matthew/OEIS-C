/*
 * A240184  Number of partitions of n such that (greatest part) >
 *          (multiplicity of least part).
 * https://oeis.org/A240184
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = p(n) - b(n), where p(n) is the unrestricted partition
 *            number of n, b(n) counts the partitions of n whose largest part
 *            ≤ the multiplicity of the smallest part
 * Range:     u8 n <= 17, u16 n <= 44, u32 n <= 47, u64 n <= 47, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

seq_t a(idx_t n) {
    if (n == 0) return 0;   /* a(0) = 0 by definition */

    /* ---------- compute unrestricted partitions p(n) ---------- */
    seq_t *P = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!P) return 0;   /* allocation failure */
    P[0] = 1;
    for (idx_t add = 1; add <= n; ++add) {
        for (idx_t sum = add; sum <= n; ++sum) {
            P[sum] += P[sum - add];
        }
    }
    seq_t total = P[n];
    free(P);

    /* ---------- compute b(n) : partitions with largest part ≤ multiplicity of smallest part ---------- */
    seq_t complement = 0;
    seq_t *dp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp) return 0;   /* allocation failure */

    for (idx_t s = 1; s <= n; ++s) {
        idx_t upper = n / s;          /* largest possible multiplicity */
        if (upper < s) continue;     /* no valid m with m*s ≤ n */

        /* dp[t] = partitions of t using parts in [s+1, current_m] */
        memset(dp, 0, (size_t)(n + 1) * sizeof(seq_t));
        dp[0] = 1;                    /* empty partition */

        /* case m = s (no extra parts allowed) */
        idx_t t_val = n - s * s;
        complement += dp[t_val];      /* dp[0] is 1 if t_val == 0, else 0 */

        /* process m = s+1 .. upper */
        for (idx_t m = s + 1; m <= upper; ++m) {
            for (idx_t sum = m; sum <= n; ++sum) {
                dp[sum] += dp[sum - m];
            }
            t_val = n - m * s;
            complement += dp[t_val];
        }
    }
    free(dp);

    /* ---------- final answer ---------- */
    return total - complement;
}
