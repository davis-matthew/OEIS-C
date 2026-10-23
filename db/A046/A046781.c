/*
 * A046781  Number of partitions of n with equal nonzero number of parts
 *          congruent to each of 1, 2 and 3 (mod 4).
 * https://oeis.org/A046781
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 33, u16 n <= 71, u32 n <= 181, u64 n <= 525,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A046781: Number of partitions of n with equal nonzero number
   of parts congruent to 1, 2 and 3 (mod 4).  The formula (OEIS)
   is given by the generating function string in the description.
   The implementation computes a(n) exactly, using only integer
   operations on the type seq_t.
*/

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* fast exits for the zero/odd and small cases */
    if ((n & 1) || n < 6)          /* n odd or n < 6 → a(n)=0 */
        return (seq_t)0;

    /* maximal sum that can appear in the DP tables */
    idx_t maxSum = n;
    size_t len = (size_t)(maxSum + 1);

    /* DP array: dp[s] = number of partitions of s into multiples of 4
       with the current number of colours for each size.  Initially only
       one colour per size is available (k = 0). */
    seq_t *dp = (seq_t *)malloc(len * sizeof(seq_t));
    if (!dp) return (seq_t)0;        /* allocation failure – safe fallback */

    dp[0] = (seq_t)1;
    for (size_t i = 1; i < len; ++i) dp[i] = (seq_t)0;

    /* Baseline: one colour per size 4, 8, 12, ... */
    for (idx_t val = 4; val <= maxSum; val += 4) {
        for (idx_t s = val; s <= maxSum; ++s) {
            dp[s] += dp[s - val];
        }
    }

    seq_t result = (seq_t)0;
    idx_t maxK = n / 6;             /* largest possible k with 6*k <= n */

    for (idx_t k = 1; k <= maxK; ++k) {
        /* For each new k we add 3 additional colours for size 4*k */
        idx_t val = 4 * k;
        for (int c = 0; c < 3; ++c) {
            for (idx_t s = val; s <= maxSum; ++s) {
                dp[s] += dp[s - val];
            }
        }

        /* Remaining sum after removing 6*k (in words of the partition) */
        idx_t rem = n - 6 * k;
        if ((rem & 3) == 0) {       /* rem divisible by 4 */
            result += dp[rem];
        }
    }

    free(dp);
    return result;
}
