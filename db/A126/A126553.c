/*
 * A126553  Number of base 32 n-digit numbers with adjacent digits differing
 *          by five or less.
 * https://oeis.org/A126553
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   * a(0) = 1, * For n ≥ 1: let dp[n][d] be the number of length-n
 *            base‑32 strings ending with digit d * (digits 0…31, leading
 *            zeros allowed). * dp[1][d] = 1 for all d. * dp[i][d] = Σ
 *            dp[i‑1][k] (k ranges over digits with |k‑d| ≤ 5). * a(n) = Σ
 *            dp[n][d]. * This counts all n‑digit base‑32 strings wit...
 * Range:     u8 n <= 1, u16 n <= 4, u32 n <= 8, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdlib.h>

static seq_t *a_cache = NULL;        /* cached a(n) values  */
static size_t cache_len = 0;         /* number of cached entries  */
static size_t cache_cap = 0;         /* allocated capacity  */

static seq_t dp[32];                 /* DP state for the last computed length  */
static size_t dp_len = 0;            /* current length represented by dp  */

/* Ensure cache has space for at least new_cap entries */
static void ensure_cache_cap(size_t new_cap)
{
    if (new_cap <= cache_cap) return;
    size_t cap = cache_cap ? cache_cap : 8;
    while (cap < new_cap) cap <<= 1;
    seq_t *tmp = (seq_t *)realloc(a_cache, cap * sizeof(seq_t));
    if (!tmp) exit(EXIT_FAILURE);  /* memory exhausted */
    a_cache = tmp;
    cache_cap = cap;
}

/* Extend the cache (and DP state) up to and including n */
static void extend_cache(idx_t n)
{
    /* 1. Ensure space for a(n) */
    ensure_cache_cap((size_t)n + 1);

    /* 2. Handle the first call: initialize a(0) */
    if (cache_len == 0) {
        a_cache[0] = (seq_t)1;
        cache_len = 1;
    }

    /* 3. If we need values beyond a(0), initialise length‑1 DP */
    if (cache_len <= 1) {
        for (int d = 0; d < 32; ++d) dp[d] = (seq_t)1;
        dp_len = 1;
        seq_t sum = 0;
        for (int d = 0; d < 32; ++d) sum += dp[d];
        if (cache_len <= 1) a_cache[1] = sum;
        cache_len = 2;  /* a(0) and a(1) stored */
    }

    /* 4. Extend for each length from current cache_len to n */
    for (size_t cur = cache_len; cur <= (size_t)n; ++cur) {
        seq_t nextdp[32];
        for (int d = 0; d < 32; ++d) {
            int low  = d - 5;
            if (low < 0) low = 0;
            int high = d + 5;
            if (high > 31) high = 31;
            seq_t total = 0;
            for (int k = low; k <= high; ++k) total += dp[k];
            nextdp[d] = total;
        }
        /* copy nextdp back to dp */
        for (int d = 0; d < 32; ++d) dp[d] = nextdp[d];
        ++dp_len;  /* dp now represents length cur */

        /* sum dp to get a(cur) */
        seq_t total = 0;
        for (int d = 0; d < 32; ++d) total += dp[d];
        a_cache[cur] = total;
        ++cache_len;
    }
}

seq_t a(idx_t n)
{
    if ((size_t)n >= cache_len) extend_cache(n);
    return a_cache[(size_t)n];
}
