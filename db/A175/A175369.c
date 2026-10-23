/*
 * A175369  Partial sums of A175368.
 * https://oeis.org/A175369
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Count of integer 4‑tuples (x,y,z,u) with
   |x|³+|y|³+|z|³+|u|³ <= n and -n <= x,y,z,u <= n.
   The implementation uses a dynamic‑programming DP over
   the possible non‑negative absolute values.
   Each coordinate contributes weight 1 when the absolute
   value is 0 and weight 2 otherwise (for the two signs).   */

#include <stdlib.h>

static inline idx_t floor_cube_root(idx_t n) {
    idx_t r = 0;
    while ((r + 1) * (r + 1) * (r + 1) <= n)
        ++r;
    return r;
}

seq_t a(idx_t n) {
    /* Pre‑compute the largest needed cube root */
    idx_t maxC = floor_cube_root(n);
    /* Allocate and fill cube values */
    idx_t *cube = (idx_t *)malloc((size_t)(maxC + 1) * sizeof(idx_t));
    for (idx_t i = 0; i <= maxC; ++i)
        cube[i] = i * i * i;

    /* DP array for exact sums */
    seq_t *dp  = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    dp[0] = 1;                /* empty tuple contributes 1 */

    /* Process the four coordinates independently */
    for (idx_t coord = 0; coord < 4; ++coord) {
        seq_t *ndp = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
        for (idx_t sum = 0; sum <= n; ++sum) {
            if (dp[sum] == 0) continue;
            seq_t base = dp[sum];
            for (idx_t v = 0; v <= maxC; ++v) {
                idx_t c = cube[v];
                if (c > n - sum) break;
                seq_t weight = (v == 0) ? (seq_t)1 : (seq_t)2;
                ndp[sum + c] += base * weight;
            }
        }
        free(dp);
        dp = ndp;
    }

    /* Accumulate all sums <= n to get the final count */
    seq_t result = 0;
    for (idx_t s = 0; s <= n; ++s)
        result += dp[s];

    /* Clean up */
    free(dp);
    free(cube);

    return result;
}
