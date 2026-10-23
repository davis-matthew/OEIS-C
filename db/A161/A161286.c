/*
 * A161286  Number of partitions of n into numbers not divisible by 3 where
 *          every part appears at least 8 times.
 * https://oeis.org/A161286
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 224, u32 n <= 637, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The number of partitions of n into numbers not divisible by 3,
   where each part appears at least 8 times. */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* For n < 8 there cannot be a part appearing at least 8 times. */
    if (n < (idx_t)8) return (seq_t)0;

    /* Convert n to a size_t for array bounds.  n is <= 10000 for the
       specified test limits, so the cast is safe. */
    const size_t N = (size_t)n;

    /* Allocate DP arrays; cast the void* from calloc to the appropriate
       pointer type for C++ compatibility. */
    seq_t *dp1 = (seq_t *)calloc(N + 1, sizeof(seq_t));
    seq_t *dp2 = (seq_t *)calloc(N + 1, sizeof(seq_t));
    if (!dp1 || !dp2) return (seq_t)0;   /* allocation failed */

    dp1[0] = (seq_t)1;                   /* one way to partition 0 */

    /* Iterate over all allowed part sizes k (k ≢ 0 (mod 3)). */
    for (idx_t k = 1; k <= n; ++k) {
        if (k % 3 == 0) continue;       /* skip multiples of 3 */
        if ((idx_t)8 * k > n) break;    /* larger k can never be used */

        /* Base case: copy current DP to dp2 (using 0 copies of k). */
        for (size_t i = 0; i <= N; ++i) dp2[i] = dp1[i];

        /* Handle residues modulo k.  For each residue r we
           accumulate contributions from using 8 or more copies of part k. */
        for (idx_t r = 0; r < k; ++r) {
            seq_t sum = (seq_t)0;
            for (idx_t m = 0; ; ++m) {
                idx_t idx = r + m * k;
                if (idx > n) break;
                if (m >= 8) {                           /* at least 8 copies */
                    idx_t prev = idx - (idx_t)8 * k;    /* idx - 8k */
                    sum += dp1[prev];
                }
                dp2[idx] += sum;
            }
        }

        /* Prepare for next part: dp1 holds the new DP values. */
        seq_t *tmp = dp1;
        dp1 = dp2;
        dp2 = tmp;
    }

    seq_t result = dp1[N];
    free(dp1);
    free(dp2);
    return result;
}
