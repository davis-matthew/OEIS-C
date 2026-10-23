/*
 * A182887  Number of (1,0)-steps in all weighted lattice paths in L_n.
 * https://oeis.org/A182887
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A182887: Number of (1,0)-steps in all weighted lattice paths in L_n
   Computed by dynamic programming on weight and height.  The recurrence is
   equivalent to the conjectured 5th‑order linear recurrence with polynomial
   coefficients.  The implementation never uses large intermediate
   multiplications and works for any seq_t type the build supplies. */
#include <stdlib.h>
#include <stdint.h>

seq_t a(idx_t n)
{
    /* special case: start at (0,0) , weight 0, empty path */
    if (n == 0) return 0;

    size_t maxsize   = 2 * n + 1;      /* heights range from -n .. n        */
    size_t offset    = n;              /* index of height 0                 */

    /* three buffers allow us to keep only the last two rows            */
    seq_t **dp    = (seq_t**)malloc(3 * sizeof(seq_t*));
    seq_t **sumdp = (seq_t**)malloc(3 * sizeof(seq_t*));
    for (int i = 0; i < 3; ++i) {
        dp[i]    = (seq_t*)calloc(maxsize, sizeof(seq_t));
        sumdp[i] = (seq_t*)calloc(maxsize, sizeof(seq_t));
    }

    /* initial row: weight 0, only one empty path at height 0            */
    dp[0][offset]    = 1;
    sumdp[0][offset] = 0;

    /* iterate over all weights up to n                               */
    for (idx_t w = 1; w <= n; ++w) {
        int cur   = w % 3;
        int prev1 = (w - 1) % 3;
        int prev2 = (w >= 2) ? ((w - 2) % 3) : 0;   /* unused when w < 2 */

        /* heights from -w to w are needed                          */
        for (int h = -(int)w; h <= (int)w; ++h) {
            size_t idx = (size_t)(h + (int)offset);
            seq_t val = 0;
            seq_t sum = 0;

            /* horizontal weight 1, (1,0) steps of weight 1          */
            val  += dp[prev1][idx];
            sum += sumdp[prev1][idx] + dp[prev1][idx];

            /* horizontal weight 2 (horizontals of weight 2)        */
            if (w >= 2) {
                val  += dp[prev2][idx];
                sum += sumdp[prev2][idx] + dp[prev2][idx];

                /* up step (1,1) weight 2                            */
                if (idx > 0) {               /* idx-1 ≥ 0 */
                    val  += dp[prev2][idx - 1];
                    sum += sumdp[prev2][idx - 1];
                }
            }

            /* down step (1,-1) weight 1                            */
            if (idx + 1 < maxsize) {
                val  += dp[prev1][idx + 1];
                sum += sumdp[prev1][idx + 1];
            }

            dp[cur][idx]    = val;
            sumdp[cur][idx] = sum;
        }
    }

    seq_t result = sumdp[n % 3][offset];

    /* cleanup                                                          */
    for (int i = 0; i < 3; ++i) {
        free(dp[i]);    free(sumdp[i]);
    }
    free(dp);   free(sumdp);

    return result;
}
