/*
 * A092130  Number of partitions of n into distinct parts == 1 (mod 3), with 1
 *          as the smallest part.
 * https://oeis.org/A092130
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into distinct parts ≡ 1 (mod 3)
 *            with the part 1 always present. The generating function is
 *            x·∏_{k≥1}(1 + x^(1+3k)). An O(n²) dynamic‑programming approach
 *            counts partitions into the allowed parts 4,7,10,…, using each
 *            part at most once
 * Range:     u8 n <= 116, u16 n <= 280, u32 n <= 786, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>

/* Forward declaration – types are supplied by the build. */
extern void *xzmalloc(size_t);  /* if needed */

/* Internal state: dynamic array of dp values. */
static seq_t *dp = NULL;    /* dp[s] = number of ways to write s using parts 4,7,10,… */
static idx_t dp_cap = 0;    /* current capacity of dp (≥ needed size) */

/* Recompute dp up to all sums ≤ size‑1 (i.e., part of n ≤ size). */
static void recompute(idx_t size)
{
    if (dp_cap < size) {   /* enlarge array */
        /* free old array if any */
        free(dp);
        dp = (seq_t *)malloc((size_t)size * sizeof(seq_t));
        dp_cap = size;
    }

    /* initialise dp to zero */
    memset(dp, 0, (size_t)size * sizeof(seq_t));
    dp[0] = 1;            /* empty sum exists */

    /* iterate over allowed distinct parts: 4,7,10,… ≤ size‑1 */
    for (idx_t part = 4; part < size; part += 3) {
        for (idx_t j = size - 1; j >= part; --j) {
            dp[j] = dp[j] + dp[j - part];
        }
    }
}

/* Public function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0) return 0;          /* offset is 1 – 0 is outside the sequence */
    /* we need sums up to n‑1 */
    idx_t need = n;
    if (dp_cap < need) recompute(need);
    return dp[n - 1];
}
