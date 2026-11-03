/*
 * A078635  Number of partitions of n into perfect powers.
 * https://oeis.org/A078635
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of n into perfect powers. (Using
 *            dynamic programming with coin change over all distinct perfect
 *            powers.)
 * Range:     u8 n <= 48, u16 n <= 155, u32 n <= 616, u64 n <= 2976,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9948 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

/* Generate all distinct perfect powers <= limit.  Result is stored in powers array.
   The array is malloc'ed/realloc'ed as needed.  *len is set to number of elements.
   *cap may be increased by the function. */
static void generate_powers(idx_t limit, idx_t **powers, size_t *len, size_t *cap)
{
    if (*cap == 0) {
        *cap = 1024;
        *powers = (idx_t*)malloc(*cap * sizeof(idx_t));
    }
    *len = 0;

    /* Mark array to avoid duplicates */
    unsigned char *marked = (unsigned char*)calloc((size_t)(limit + 1), 1);
    if (!marked) return;  /* allocation failed – unlikely for limits <= 10000 */

    /* 1 is a perfect power (1^j) */
    if (*len == *cap) { *cap *= 2; *powers = (idx_t*)realloc(*powers, *cap * sizeof(idx_t)); }
    (*powers)[(*len)++] = 1;

    for (idx_t base = 2; base <= limit; ++base) {
        idx_t val = base * base;          /* base^2 */
        while (val <= limit) {
            if (!marked[(size_t)val]) {
                marked[(size_t)val] = 1;
                if (*len == *cap) { *cap *= 2; *powers = (idx_t*)realloc(*powers, *cap * sizeof(idx_t)); }
                (*powers)[(*len)++] = val;
            }
            if (val > limit / base) break;   /* avoid overflow */
            val *= base;
        }
    }
    free(marked);
}

static seq_t *dp = NULL;          /* DP array */
static idx_t  max_computed = 0;   /* maximum n for which dp was computed */

/* Recompute dp array up to new_limit using all perfect powers <= new_limit. */
static void recompute_dp(idx_t new_limit)
{
    /* Allocate new dp array */
    dp = mem_new(seq_t, (size_t)(new_limit + 1));
    if (!dp) return;  /* allocation failed – unlikely */

    dp[0] = (seq_t)1;   /* base case: one way to partition 0 */

    /* Generate perfect powers up to new_limit */
    idx_t *powers = NULL;
    size_t p_len = 0, p_cap = 0;
    generate_powers(new_limit, &powers, &p_len, &p_cap);

    /* Simple coin–change DP */
    for (size_t p_idx = 0; p_idx < p_len; ++p_idx) {
        idx_t coin = powers[p_idx];
        for (idx_t s = coin; s <= new_limit; ++s) {
            dp[s] += dp[s - coin];
        }
    }

    free(powers);
    max_computed = new_limit;
}

/* Public function */
seq_t a(idx_t n)
{
    if (n > max_computed || dp == NULL) {
        recompute_dp(n);
    }
    return dp[(size_t)n];
}
