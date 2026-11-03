/*
 * A025463  Number of partitions of n into 10 positive cubes.
 * https://oeis.org/A025463
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [x^n y^10] ∏_{k≥1} 1/(1−y·x^{k^3})
 * Range:     u8 n <= 6696, u16 n <= 65536, u32 n <= 65536, u64 n <= 65536,
 *            u128 n <= 65536, big any n
 * Validated: standard, pass, 124 terms through n = 9820 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* Helper: allocate an array of seq_t and zero it. */
static seq_t *alloc_seq(size_t n)
{
    return mem_new(seq_t, n);
}

/* Helper: free an array allocated by alloc_seq. */
static void free_seq(seq_t *p)
{
    mem_free(p);
}

/* Number of partitions of n into exactly ten positive cubes.  */
seq_t a(idx_t n)
{
    /* Exactly ten cubes are required.  */
    const int C   = 10;               /* number of cubes                 */
    const size_t W = (size_t)(C + 1);  /* width of DP table (k = 0..10)   */

    /* DP table of size (n+1) × (C+1).  */
    size_t tot = (size_t)(n + 1) * W;
    seq_t *dp  = alloc_seq(tot);
    if (!dp)
        return (seq_t)0;  /* allocation failure → return 0 */

    /* initialise to zero (alloc_seq may already do this, but be safe) */
    mem_set(dp, 0, tot * sizeof(seq_t));

    /* dp[0][0] = 1 : one way to form 0 using 0 cubes */
    dp[0] = (seq_t)1;

    /* iterate over all possible cube values k^3 <= n */
    for (idx_t k = 1;; ++k) {
        idx_t cube_val = k * k * k;
        if (cube_val > n)
            break;                /* no larger cubes needed */

        /* unbounded knapsack: add one more cube to existing ways */
        for (idx_t sum = cube_val; sum <= n; ++sum) {
            size_t cur  = (size_t)sum * W;
            size_t prev = (size_t)(sum - cube_val) * W;

            for (int cnt = 1; cnt <= C; ++cnt)
                dp[cur + cnt] += dp[prev + cnt - 1];
        }
    }

    seq_t result = dp[(size_t)n * W + C];
    free_seq(dp);
    return result;
}
