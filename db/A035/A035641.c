/*
 * A035641  Number of partitions of n into parts 6k and 6k+5 with at least one
 *          part of each type.
 * https://oeis.org/A035641
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 6k and 6k+5 with at
 *            least one part of each type
 * Range:     u8 n <= 69, u16 n <= 163, u32 n <= 444, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static idx_t max_computed = 0;            /* largest n for which tables are built */

#ifdef __cplusplus
#include <vector>
static std::vector<seq_t> dp[4];
#else
static seq_t *dp[4] = {NULL, NULL, NULL, NULL};
#endif

/* Rebuild the DP tables up to N (inclusive). Complexity: O(N²). */
static void rebuild(idx_t N)
{
#ifdef __cplusplus
    const size_t sz = (size_t)(N + 1);
    for (int m = 0; m < 4; ++m) {
        dp[m].assign(sz, 0);              /* zero‑initialize */
    }
    dp[0][0] = 1;                          /* empty partition, no types used */

    for (idx_t p = 5; p <= N; ++p) {
        int mask_new = 0;
        if (p % 6 == 0)          mask_new = 1;   /* 6k part  */
        else if (p % 6 == 5)     mask_new = 2;   /* 6k+5 part */

        if (mask_new == 0)       continue;      /* skip other residues */

        for (idx_t s = p; s <= N; ++s) {
            for (int old_mask = 0; old_mask < 4; ++old_mask) {
                seq_t val = dp[old_mask][s - p];
                if (val != 0) {
                    dp[old_mask | mask_new][s] += val;
                }
            }
        }
    }
#else /* C implementation */
    for (int m = 0; m < 4; ++m) {
        seq_t *arr = (seq_t *)realloc(dp[m], (N + 1) * sizeof(seq_t));
        if (!arr) exit(1);                     /* allocation failure aborts */
        dp[m] = arr;
        for (idx_t i = 0; i <= N; ++i) dp[m][i] = 0; /* zero‑initialize */
    }
    dp[0][0] = 1;                              /* empty partition, no types used */

    for (idx_t p = 5; p <= N; ++p) {
        int mask_new = 0;
        if (p % 6 == 0)          mask_new = 1;   /* 6k part  */
        else if (p % 6 == 5)     mask_new = 2;   /* 6k+5 part */

        if (mask_new == 0)       continue;      /* skip other residues */

        for (idx_t s = p; s <= N; ++s) {
            for (int old_mask = 0; old_mask < 4; ++old_mask) {
                seq_t val = dp[old_mask][s - p];
                if (val != 0) {
                    dp[old_mask | mask_new][s] += val;
                }
            }
        }
    }
#endif
    max_computed = N;
}

/* Public sequence function */
seq_t a(idx_t n)
{
    if (n == 0) return 0;                       /* safety for index 0 */
    if (n > max_computed) rebuild(n);
    return dp[3][n];                            /* both types present */
}
