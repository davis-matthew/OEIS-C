/*
 * A091893  Number of partitions of n into numbers having all the same number
 *          of 1's in binary representation.
 * https://oeis.org/A091893
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into parts that all have the
 *            same number of 1-bits in their binary representation
 * Range:     u8 n <= 31, u16 n <= 115, u32 n <= 392, u64 n <= 1472,
 *            u128 n <= 5434, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>

static inline idx_t popcount_u64(idx_t x)
{
#if defined(__GNUC__) || defined(__clang__)
    return __builtin_popcountll((unsigned long long)x);
#else
    /* fallback: count bits manually */
    idx_t c = 0;
    while (x) { c++; x &= x - 1; }
    return c;
#endif
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* only defined for n >= 1, guard */

    /* maximum possible popcount for numbers up to n */
    idx_t max_k = 0;
    for (idx_t m = 1; m <= n; ++m) {
        idx_t k = popcount_u64(m);
        if (k > max_k) max_k = k;
    }
    if (max_k == 0) return (seq_t)1;       /* n == 1 */

    /* allocate a contiguous buffer for all dp tables: (max_k+1) * (n+1) cells */
    size_t total_cells = (size_t)(max_k + 1) * (size_t)(n + 1);
    seq_t *buffer;
#ifdef __cplusplus
    buffer = static_cast<seq_t*>(calloc(total_cells, sizeof(seq_t)));
#else
    buffer = (seq_t*)calloc(total_cells, sizeof(seq_t));
#endif
    if (!buffer) return (seq_t)0;           /* out of memory */

    /* array of pointers to each dp table */
    seq_t **dp;
#ifdef __cplusplus
    dp = static_cast<seq_t**>(malloc((size_t)(max_k + 1) * sizeof(seq_t*)));
#else
    dp = (seq_t**)malloc((size_t)(max_k + 1) * sizeof(seq_t*));
#endif
    if (!dp) { free(buffer); return (seq_t)0; }

    for (idx_t k = 0; k <= max_k; ++k) {
        dp[k] = buffer + (size_t)k * (size_t)(n + 1);
        dp[k][0] = (seq_t)1;               /* empty partition counts as one way */
    }

    /* classic coin‑change DP for each popcount class */
    for (idx_t m = 1; m <= n; ++m) {
        idx_t k = popcount_u64(m);
        seq_t *dp_k = dp[k];
        for (idx_t s = m; s <= n; ++s) {
            dp_k[s] += dp_k[s - m];
        }
    }

    /* sum contributions from all popcount classes */
    seq_t result = (seq_t)0;
    for (idx_t k = 1; k <= max_k; ++k) {
        result += dp[k][n];
    }

    /* clean up */
    free(dp);
    free(buffer);
    return result;
}
