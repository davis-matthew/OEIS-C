/*
 * A117930  Number of partitions of 2n into factorial parts (0! not allowed,
 *          i.e., only one kind of 1 can be a part). Also number of partitions
 *          of 2n+1 into factorial parts.
 * https://oeis.org/A117930
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of partitions of 2n into factorial parts (0! not
 *            allowed, only one kind of 1 can be a part)
 * Range:     u8 n <= 29, u16 n <= 204, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Coin‑change DP: for each factorial p, add dp[s-p] to dp[s] for all s≥p. */
#include <stdlib.h>

/* Static cache for DP results. */
static seq_t *dp_cache = NULL;
static size_t dp_size = 0;

/* Predefined factorial parts (1!,2!,3!,4!,5!,6!,7!). */
static const seq_t factorial_parts[] = {
    (seq_t)1, (seq_t)2, (seq_t)6, (seq_t)24,
    (seq_t)120, (seq_t)720, (seq_t)5040
};
static const size_t num_factorials =
    sizeof(factorial_parts) / sizeof(factorial_parts[0]);

/* Ensure dp_cache contains partition counts up to 'target' (inclusive). */
static void ensure_dp(idx_t target) {
    size_t new_max = (size_t)target + 1;
    if (dp_cache == NULL || new_max > dp_size) {
        /* Allocate new cache. */
        seq_t *new_cache = (seq_t *)malloc(new_max * sizeof(seq_t));
        if (!new_cache) abort();          /* Failed allocation. */

        /* Zero out and compute DP. */
        for (size_t i = 0; i < new_max; ++i)
            new_cache[i] = 0;
        new_cache[0] = 1;

        for (size_t fp = 0; fp < num_factorials; ++fp) {
            idx_t p = (idx_t)factorial_parts[fp];
            if (p > target) break;          /* Remaining factorials are larger. */
            for (idx_t s = p; s <= target; ++s) {
                new_cache[(size_t)s] +=
                    new_cache[(size_t)(s - p)];
            }
        }

        /* Replace old cache. */
        free(dp_cache);
        dp_cache = new_cache;
        dp_size = new_max;
    }
}

seq_t a(idx_t n) {
    idx_t target = 2 * n;
    ensure_dp(target);
    return dp_cache[(size_t)target];
}
