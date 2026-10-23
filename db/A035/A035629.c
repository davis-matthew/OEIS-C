/*
 * A035629  Number of partitions of n into parts 5k and 5k+3 with at least one
 *          part of each type.
 * https://oeis.org/A035629
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 53, u16 n <= 126, u32 n <= 349, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A035629: Number of partitions of n into parts 5k and 5k+3 with at least one part of each type.
   Computation uses a standard coin–change DP keeping two flags.
   The DP is O(n²) with a constant factor about 4, well below the time limit for n ≤ 10000. */
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;            /* For safety; offset starts at 1 */

    /* Allocate dp array: (n+1) rows, 4 columns for flags (00,01,10,11) */
    const size_t rows = (size_t)n + 1;
    const size_t cols = 4;
    seq_t *dp = (seq_t *)malloc(rows * cols * sizeof(seq_t));
    if (!dp) return 0;               /* Out of memory – return 0 as a safe default */

    /* Helper macro for indexing: dp[row * 4 + col] */
    #define DP(r,c) (*((dp + (size_t)(r) * cols + (c))))

    /* Initialise */
    for (size_t i = 0; i < rows * cols; ++i) dp[i] = 0;
    DP(0, 0) = 1;                     /* Empty partition, no type used */

    /* Process all part sizes in increasing order: 3,5,8,10,13,15,... up to n */
    for (idx_t p = 3; p <= n; ++p) {
        /* Identify if p is of type 5k (mask 1) or 5k+3 (mask 2) */
        int mask_part;
        if (p % 5 == 0) {             /* p = 5k, k ≥ 1 */
            if (p < 5) continue;      /* skip 0 */
            mask_part = 1;
        } else if (p % 5 == 3) {       /* p = 5k+3, k ≥ 0 */
            mask_part = 2;
        } else {
            continue;                 /* Not a valid part size */
        }

        /* Update dp for this part (unlimited supply) */
        for (idx_t s = p; s <= n; ++s) {
            idx_t prev = s - p;
            for (int m = 0; m < 4; ++m) {
                int newmask = m | mask_part;
                DP(s, newmask) += DP(prev, m);
            }
        }
    }

    seq_t result = DP(n, 3);  /* mask 3 means both types used at least once */
    free(dp);
    return result;
}
