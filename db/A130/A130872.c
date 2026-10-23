/*
 * A130872  Number of ways to write 8n - 1 as a sum of 7 (not necessarily
 *          distinct) odd squares.
 * https://oeis.org/A130872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of ways to write 8n-1 as a sum of 7 odd squares
 * Range:     u8 n <= 130, u16 n <= 1819, u32 n <= 9886, u64 n <= 9886,
 *            u128 n <= 9886, big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

seq_t a(idx_t n)
{
    static seq_t *dp[8] = {NULL};
    static idx_t dpSize = 0;                 /* largest target that has been processed */

    if (n == 0) return (seq_t)0;              /* safety; sequence starts at n = 1 */

    idx_t target = (idx_t)n * (idx_t)8 - (idx_t)1;  /* target = 8·n – 1 */

    if (target > dpSize) {                    /* need to recompute for a larger target */
        /* free any previously allocated rows */
        for (int j = 0; j < 8; ++j) {
            if (dp[j]) {
                free_seq_array(dp[j]);
                dp[j] = NULL;
            }
        }

        /* allocate new rows of size (target+1) and zero‑initialise them */
        size_t sz = (size_t)(target + 1);
        for (int j = 0; j < 8; ++j) {
            dp[j] = alloc_seq_array(sz);       /* allocates and default‑constructs elements */
            if (!dp[j]) return (seq_t)0;      /* allocation failure → return 0 */

            /* ensure explicit zero‑initialisation for all elements (safety for large types) */
            for (idx_t i = 0; i <= target; ++i)
                dp[j][i] = (seq_t)0;
        }

        /* base case: 0 squares sum to 0 */
        dp[0][0] = (seq_t)1;

        /* dynamic programming over all odd squares ≤ target */
        for (idx_t k = 1; k * k <= target; k += 2) {
            idx_t v = k * k;                 /* current odd square value */
            for (int j = 1; j <= 7; ++j) {
                for (idx_t m = v; m <= target; ++m) {
                    dp[j][m] += dp[j - 1][m - v];
                }
            }
        }

        dpSize = target;
    }

    return dp[7][target];
}
