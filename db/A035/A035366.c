/*
 * A035366  Number of partitions of n into parts 4k+2 or 4k+3.
 * https://oeis.org/A035366
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 58, u32 n <= 58, u64 n <= 58, u128 n <= 58,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035366: number of partitions of n into parts congruent to 2 or 3 (mod 4) */

/* Implementation: classic one‑dimensional DP.
 * For every part p = 2,3,6,7,... up to n we add its contribution:
 *   dp[s] += dp[s–p]  for all s ≥ p.
 * dp[0] = 1 (empty partition) and the answer is dp[n].
 * The table is stored in a static buffer which is resized only when a
 * larger n than previously seen is requested.  The function recomputes
 * the table from scratch on each call; this guarantees correct results
 * even for calls in arbitrary order while staying well below the time
 * limit for n ≤ 10000.  All arithmetic is performed in seq_t, which
 * satisfies the required support for 8, 16, 32, 64 or 128‑bit integers
 * or arbitrary precision.  No floating point or external libraries are
 * used. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static table shared across calls */
    static seq_t *dp = NULL;
    static idx_t dp_size = 0;    /* number of entries allocated */

    if (n == 0)          /* offset is 1; a(0) is undefined, but return 0 */
        return 0;

    /* ensure buffer is large enough */
    if (dp_size <= n) {
        seq_t *tmp = (seq_t *)realloc(dp, (size_t)(n + 1) * sizeof(seq_t));
        if (!tmp) return 0;      /* allocation failure */
        dp = tmp;
        dp_size = n + 1;
    }

    /* initialise DP array */
    dp[0] = 1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = 0;

    /* dynamic programming: iterate over allowed parts */
    for (idx_t p = 2; p <= n; ++p) {
        if ((p & 3) == 2 || (p & 3) == 3) {              /* p ≡ 2 or 3 (mod 4) */
            for (idx_t s = p; s <= n; ++s) {
                dp[s] += dp[s - p];
            }
        }
    }
    return dp[n];
}
