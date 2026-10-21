/*
 * A229541  Number T(n,k) of partitions of n^2 into squares with each number
 *          of parts k; irregular triangle T(n,k), 1 <= k <= n^2.
 * https://oeis.org/A229541
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf
 * Formula:   a(n) = T(row, col) -- the entry in the irregular triangle of the
 *            number of partitions of row^2 into exactly col squares. The
 *            triangle is flattened in row-major order
 * Range:     u8 n <= 674, u16 n <= 3311, u32 n <= 3311, u64 n <= 3311,
 *            u128 n <= 3311, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Include required headers for standard facilities */
#include <stdlib.h>
#include <stddef.h>

/* Helper function to ensure capacity of an array of seq_t */
static void ensure_cache_capacity(size_t new_capacity, seq_t **ptr, size_t *cap) {
    if (new_capacity <= *cap) return;
    size_t new_cap = (*cap == 0) ? new_capacity : (*cap << 1);
    while (new_cap < new_capacity) new_cap <<= 1;
    seq_t *tmp = (seq_t*)realloc(*ptr, new_cap * sizeof(seq_t));
    if (!tmp) exit(1);                /* simple error handling */
    *ptr = tmp;
    *cap = new_cap;
}

/* The primary function.  Static state keeps the cache between calls. */
seq_t a(idx_t n) {
    /* Static cache of flattened triangle values */
    static seq_t *cache = NULL;
    static size_t cache_len = 0;      /* number of values already cached */
    static size_t cache_cap = 0;      /* capacity of the cache array */
    static idx_t next_row = 1;        /* next row to be computed */

    /* If the required value is already cached, return it immediately. */
    if (n <= cache_len) return cache[n - 1];

    /* Compute rows until we have enough entries. */
    while (cache_len < (size_t)n) {
        idx_t row = next_row;
        idx_t N   = row * row;          /* N = row^2, max sum */
        size_t S  = (size_t)N + 1;      /* dimension of DP table */
        size_t DPsize = S * S;

        /* DP table: dp[s * S + k] == partitions of s using exactly k squares */
        seq_t *dp = (seq_t*)calloc(DPsize, sizeof(seq_t));
        if (!dp) exit(1);
        dp[0] = (seq_t)1;                /* dp[0][0] = 1 */

        /* Process each square value as an "unlimited coin" */
        for (size_t val = 1; val * val <= (size_t)N; ++val) {
            size_t square = val * val;
            for (size_t s = square; s <= (size_t)N; ++s) {
                size_t base_curr = s * S;
                size_t base_prev = (s - square) * S;
                /* maximum possible number of parts for sum s is s */
                size_t max_k = s;
                for (size_t k = 1; k <= max_k; ++k) {
                    dp[base_curr + k] += dp[base_prev + (k - 1)];
                }
            }
        }

        /* Append the row's entries to the cache (k = 1..N) */
        size_t required = cache_len + (size_t)N;
        ensure_cache_capacity(required, &cache, &cache_cap);
        size_t row_base = (size_t)N * S;
        for (idx_t k = 1; k <= (idx_t)N; ++k) {
            cache[cache_len++] = dp[row_base + k];
        }
        free(dp);
        ++next_row;
    }
    return cache[n - 1];
}
