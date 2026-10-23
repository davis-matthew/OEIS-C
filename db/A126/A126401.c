/*
 * A126401  Number of base 14 n-digit numbers with adjacent digits differing
 *          by two or less.
 * https://oeis.org/A126401
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of base‑14 n‑digit strings with adjacent digits
 *            differing by at most two. Recurrence: a(0) = 1 For n ≥ 1: let
 *            dp[n][d] = number of length‑n strings ending in digit d (0 ≤ d ≤
 *            13) dp[1][d] = 1 (all digits are allowed) dp[n+1][d] = Σ
 *            dp[n][k] over k with |k – d| ≤ 2 a(n) = Σ dp[n][d]...
 * Range:     u8 n <= 2, u16 n <= 6, u32 n <= 13, u64 n <= 21, u128 n <= 21,
 *            big any n
 * Helpers:   alloc_seq_array, free_seq_array
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

#include "../utils/alloc_seq_array.h"
#include "../utils/free_seq_array.h"

static seq_t *cache = NULL;          /* array a[0..] */
static size_t cache_cap = 0;          /* allocated size of cache */
static idx_t cache_len = 0;           /* number of valid entries in cache */
static idx_t max_computed = 0;        /* largest index computed so far */

static seq_t prev[14];
static seq_t cur [14];
static bool initialized = false;

seq_t a(idx_t n)
{
    /* Lazy initialization of the cache and the DP state for length 1. */
    if (!initialized) {
        /* Allocate space for indices 0 and 1. */
        size_t init_cap = 2;
        cache = alloc_seq_array(init_cap);
        for (size_t i = 0; i < init_cap; ++i) {
            cache[i] = (seq_t)0;
        }
        cache[0] = (seq_t)1;

        /* DP for length 1: every digit is allowed. */
        for (int d = 0; d < 14; ++d) {
            prev[d] = (seq_t)1;
        }
        seq_t sum = (seq_t)0;
        for (int d = 0; d < 14; ++d) {
            sum += prev[d];
        }
        cache[1] = sum;

        cache_cap = init_cap;
        cache_len = 2;
        max_computed = 1;
        initialized = true;
    }

    /* Special case for n == 0. */
    if (n == 0) {
        return cache[0];
    }

    /* Expand the cache if necessary. */
    if ((idx_t)cache_cap <= n) {
        size_t new_cap = n + 1;
        if (new_cap < cache_cap * 2) {
            new_cap = cache_cap * 2;
        }
        seq_t *new_cache = alloc_seq_array(new_cap);
        for (size_t i = 0; i < cache_len; ++i) {
            new_cache[i] = cache[i];
        }
        free_seq_array(cache);
        cache = new_cache;
        cache_cap = new_cap;
    }

    /* Return already computed value. */
    if (n < cache_len) {
        return cache[n];
    }

    /* Compute values for lengths max_computed+1 .. n. */
    for (idx_t len = max_computed + 1; len <= n; ++len) {
        /* Compute cur[d] from prev. */
        for (int d = 0; d < 14; ++d) {
            seq_t total = (seq_t)0;
            int lo = d - 2;
            int hi = d + 2;
            if (lo < 0) lo = 0;
            if (hi > 13) hi = 13;
            for (int k = lo; k <= hi; ++k) {
                total += prev[k];
            }
            cur[d] = total;
        }

        /* Sum for this length and store in cache. */
        seq_t total_len = (seq_t)0;
        for (int d = 0; d < 14; ++d) {
            total_len += cur[d];
        }
        cache[len] = total_len;

        /* Prepare for next iteration. */
        for (int d = 0; d < 14; ++d) {
            prev[d] = cur[d];
        }
    }

    max_computed = n;
    cache_len = n + 1;
    return cache[n];
}
