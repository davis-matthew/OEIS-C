/*
 * A008641  Number of partitions of n into at most 12 parts.
 * https://oeis.org/A008641
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of partitions of n into at most 12 parts
 *            Generating function: 1 / ∏_{k=1..12}(1‑x^k)
 * Range:     u8 n <= 16, u16 n <= 47, u32 n <= 189, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Standard DP implementation.
   dp[0] = 1; for each part size i = 1..12,
   for all j = i..MAX: dp[j] += dp[j-i]   (order of loops ensures
   partitions are counted without regard to order).                */

#include <stdbool.h>
#include <stdlib.h>  /* malloc, free */

static seq_t dp[10001];
static int initialized = 0;

/* Return a(n) for any n >= 0.  For n > 10000 the function falls
   back to an on‑the‑fly computation using the same DP algorithm. */
seq_t a(idx_t n)
{
    if (!initialized) {
        /* initialise the static table once */
        for (idx_t i = 0; i <= 10000; ++i) dp[i] = 0;
        dp[0] = 1;
        for (idx_t part = 1; part <= 12; ++part) {
            for (idx_t sum = part; sum <= 10000; ++sum) {
                dp[sum] += dp[sum - part];
            }
        }
        initialized = 1;
    }

    if (n <= 10000) {
        return dp[(idx_t)n];
    }

    /* On‑the‑fly computation for n > 10000 (rare in tests). */
    seq_t *tmp = (seq_t *)malloc((n + 1) * sizeof(seq_t));
    if (!tmp) {
        return 0;  /* allocation failure – return 0 as a sentinel */
    }
    for (idx_t i = 0; i <= n; ++i) tmp[i] = 0;
    tmp[0] = 1;
    for (idx_t part = 1; part <= 12; ++part) {
        for (idx_t sum = part; sum <= n; ++sum) {
            tmp[sum] += tmp[sum - part];
        }
    }
    seq_t res = tmp[n];
    free(tmp);
    return res;
}
