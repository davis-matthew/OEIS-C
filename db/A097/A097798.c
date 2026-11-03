/*
 * A097798  Number of partitions of n into abundant numbers.
 * https://oeis.org/A097798
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Number of partitions of n into abundant numbers
 * Range:     u8 n <= 131, u16 n <= 299, u32 n <= 757, u64 n <= 2187,
 *            u128 n <= 7013, big any n
 * Validated: standard, pass, 116 terms through n = 9897 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: dynamic programming over the set of abundant numbers ≤ n. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include <stddef.h>
#include <math.h>
#include "../utils/mem.h"

/* C allocation helpers */
static seq_t* seq_alloc_new_array(size_t n)   { return mem_new(seq_t, n); }

static void free_seq_array(seq_t *p)
{
    mem_free(p);
}

/* Test whether k is an abundant number. */
static inline bool is_abundant(idx_t k)
{
    if (k < 2)            /* 1 is not abundant */
        return false;

    seq_t sum = (seq_t)1; /* 1 is always a proper divisor of k > 1 */

    for (idx_t i = 2; i * i <= k; ++i) {
        if (k % i == 0) {
            sum += (seq_t)i;
            idx_t j = k / i;
            if (j != i)
                sum += (seq_t)j;
        }
    }
    return sum > (seq_t)k;
}

/* Main function required by the test harness. */
seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;          /* a(0) = 1 by definition */

    /* Allocate DP array with proper construction for arbitrary‑precision types. */
    seq_t *dp = seq_alloc_new_array((size_t)n + 1);
    if (!dp)
        return (seq_t)0;          /* allocation failure – fail harmlessly */

    dp[0] = (seq_t)1;
    for (idx_t i = 1; i <= n; ++i)
        dp[i] = (seq_t)0;

    for (idx_t k = 1; k <= n; ++k) {
        if (is_abundant(k)) {
            for (idx_t j = k; j <= n; ++j)
                dp[j] += dp[j - k];
        }
    }

    seq_t result = dp[n];
    free_seq_array(dp);
    return result;
}
