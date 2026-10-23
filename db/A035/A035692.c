/*
 * A035692  Number of partitions of n into parts 8k+3 and 8k+6 with at least
 *          one part of each type.
 * https://oeis.org/A035692
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts 8k+3 and 8k+6 * with
 *            at least one part of each type. Equivalent to *
 *            (1/product_{k>=0}(1 - x^{8k+3}) - 1)*(1/product_{k>=0}(1 -
 *            x^{8k+6}) - 1)
 * Range:     u8 n <= 76, u16 n <= 184, u32 n <= 523, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* undefined index, return 0 */

    /* allocate DP arrays for parts 8k+3 and 8k+6 */
    seq_t *dp3 = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    seq_t *dp6 = (seq_t *)calloc((size_t)(n + 1), sizeof(seq_t));
    if (!dp3 || !dp6) {                    /* allocation failed */
        return (seq_t)0;
    }

    dp3[0] = (seq_t)1;                     /* empty partition */
    dp6[0] = (seq_t)1;

    /* partitions into parts 8k+3 (starting at 3, step 8) */
    for (idx_t s = 3; s <= n; s += 8) {
        for (idx_t j = s; j <= n; ++j) {
            dp3[j] += dp3[j - s];
        }
    }

    /* partitions into parts 8k+6 (starting at 6, step 8) */
    for (idx_t s = 6; s <= n; s += 8) {
        for (idx_t j = s; j <= n; ++j) {
            dp6[j] += dp6[j - s];
        }
    }

    /* convolution: at least one part of each type */
    seq_t res = (seq_t)0;
    for (idx_t i = 1; i < n; ++i) {        /* i>0 ensures part of type 3, n-i>0 ensures part of type 6 */
        res += dp3[i] * dp6[n - i];
    }

    free(dp3);
    free(dp6);
    return res;
}
