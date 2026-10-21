/*
 * A224814  Number of subsets of {1,2,...,n-9} without differences equal to 3,
 *          6 or 9.
 * https://oeis.org/A224814
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of subsets of {1,2,…,n-9} with no two * elements
 *            differing by 3, 6 or 9. Computed by a simple DP that * keeps the
 *            9 most recent choices, so O(n*2^9) time and O(2^9) * memory.
 *            Initial values: a(0)=a(1)=…=a(8)=1
 * Range:     u8 n <= 23, u16 n <= 40, u32 n <= 74, u64 n <= 143,
 *            u128 n <= 280, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

seq_t a(idx_t n) {
    if (n < 9) {
        return (seq_t)1;
    }
    idx_t m = n - 9;                  /* number of vertices to process */
    const idx_t MASK_ALL = 511;        /* 2^9 - 1 */

    seq_t dp[512]   = {0};
    seq_t next[512];                   /* temporary workspace */
    dp[0] = (seq_t)1;                  /* start with empty set */

    for (idx_t i = 1; i <= m; ++i) {
        /* clear next */
        for (idx_t j = 0; j < 512; ++j) {
            next[j] = 0;
        }
        for (idx_t mask = 0; mask < 512; ++mask) {
            seq_t val = dp[mask];
            if (!val) continue;

            /* choose to skip i (bit 0 = 0) */
            idx_t new_mask0 = ((mask << 1) & MASK_ALL);
            next[new_mask0] += val;

            /* choose to include i (bit 0 = 1) if allowed */
            if ((mask & (1 << 2)) == 0 &&
                (mask & (1 << 5)) == 0 &&
                (mask & (1 << 8)) == 0) {
                idx_t new_mask1 = new_mask0 | 1;
                next[new_mask1] += val;
            }
        }
        /* move next into dp for next iteration */
        for (idx_t j = 0; j < 512; ++j) {
            dp[j] = next[j];
        }
    }

    /* sum over all ending masks to obtain total number of independent sets */
    seq_t result = 0;
    for (idx_t j = 0; j < 512; ++j) {
        result += dp[j];
    }
    return result;
}
