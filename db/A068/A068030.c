/*
 * A068030  Number of subsets of {1,2,3,...,n} that sum to 0 mod 9.
 * https://oeis.org/A068030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of subsets of {1, 2, …, n} whose sum is 0 (mod 9).
 *            Recurrence: Let dp[i][r] be the number of subsets of {1,…,i}
 *            with sum ≡ r (mod 9). dp[0][0] = 1, dp[0][r≠0] = 0. dp[i][r] =
 *            dp[i‑1][r] + dp[i‑1][(r‑i) mod 9]. Then a(n) = dp[n][0].
 *            Complexity: O(n); memory: O(1) per call and O(n) fo...
 * Range:     u8 n <= 11, u16 n <= 19, u32 n <= 35, u64 n <= 67, u128 n <= 131,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Static cache and DP tables shared across calls. */
static seq_t *cache = NULL;      /* cache[idx] = a(idx)   */
static size_t cache_len = 0;     /* number of values stored */
static size_t cache_cap = 0;     /* allocated capacity           */

static seq_t dp_cur[9];          /* dp for current n            */
static seq_t dp_next[9];         /* temporary storage           */

/* Ensure the cache can hold at least 'needed' elements. */
static void ensure_cache_capacity(size_t needed) {
    if (needed <= cache_cap) return;
    size_t new_cap = cache_cap ? cache_cap * 2 : 128;
    while (new_cap < needed) new_cap *= 2;
    seq_t *new_cache = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
    cache = new_cache;          /* calloc's malloc semantics: realloc keeps data */
    cache_cap = new_cap;
    if (!cache) { /* allocation failed – terminate the program */
        abort();
    }
}

/* Compute a(n). */
seq_t a(idx_t n) {
    /* Initialisation on first call. */
    if (cache_len == 0) {
        for (int r = 0; r < 9; ++r) dp_cur[r] = 0;
        dp_cur[0] = 1;                      /* empty subset */
        ensure_cache_capacity(1);
        cache[0] = dp_cur[0];               /* a(0) = 1 */
        cache_len = 1;
    }

    /* Extend cache up to desired index n. */
    if (n >= cache_len) {
        for (idx_t i = cache_len; i <= n; ++i) {
            int mod = (int)(i % 9);
            for (int r = 0; r < 9; ++r) {
                int rprev = r - mod;
                rprev %= 9;
                if (rprev < 0) rprev += 9;
                dp_next[r] = dp_cur[r] + dp_cur[rprev];
            }
            for (int r = 0; r < 9; ++r) dp_cur[r] = dp_next[r];
            ensure_cache_capacity(cache_len + 1);
            cache[cache_len++] = dp_cur[0];   /* a(i) */
        }
    }

    return cache[(size_t)n];
}
