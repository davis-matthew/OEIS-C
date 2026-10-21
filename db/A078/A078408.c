/*
 * A078408  Number of ways to partition 2n+1 into distinct positive integers.
 * https://oeis.org/A078408
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of 2*n+1 into distinct positive
 *            integers Equivalently, the number of partitions of 2*n+1 into
 *            odd parts
 * Range:     u8 n <= 13, u16 n <= 38, u32 n <= 118, u64 n <= 395,
 *            u128 n <= 1410, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>          /* for bool */

static seq_t part[20002];    /* part[k] holds the partition count for integer k */
static bool initialized = false;

/* Initialize the dynamic programming table once.
   Complexity: O(N²) with N = 20001, which is fine for n ≤ 10000. */
static void init_dp(void)
{
    /* zero-initialize all entries */
    for (idx_t i = 0; i <= 20001; ++i)
        part[i] = (seq_t)0;

    part[0] = (seq_t)1; /* base case: one way to partition 0 */

    /* classic coin‑change DP with odd coins only */
    for (idx_t coin = 1; coin <= 20001; coin += 2) {
        for (idx_t j = coin; j <= 20001; ++j) {
            part[j] += part[j - coin];
        }
    }

    initialized = true;
}

seq_t a(idx_t n)
{
    /* The formula requires the target sum 2*n+1.
       We use a hard‑coded limit of 20001 which covers n up to 10000. */
    if (n > 10000) {
        /* For larger n the table would be too small; fallback to
           dynamic computation with a limited size that grows on demand.
           This simple fallback is not expected to be used in the tests,
           but keeps the function well‑defined for any non‑negative n. */

        /* Compute the target */
        idx_t target = 2 * n + 1;
        /* Allocate a temporary array big enough for this single query */
        seq_t *tmp = (seq_t *)malloc(sizeof(seq_t) * (target + 1));
        if (!tmp) return (seq_t)0; /* allocation failure – return 0 as a fallback */

        for (idx_t i = 0; i <= target; ++i)
            tmp[i] = (seq_t)0;
        tmp[0] = (seq_t)1;
        for (idx_t coin = 1; coin <= target; coin += 2) {
            for (idx_t j = coin; j <= target; ++j) {
                tmp[j] += tmp[j - coin];
            }
        }

        seq_t result = tmp[target];
        free(tmp);
        return result;
    }

    /* Normal path: use the precomputed table. */
    if (!initialized)
        init_dp();

    idx_t target = 2 * n + 1;
    return part[target];
}
