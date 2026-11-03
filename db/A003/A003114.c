/*
 * A003114  Number of partitions of n into parts 5k+1 or 5k+4.
 * https://oeis.org/A003114
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, nice
 * Formula:   G.f. = Product_{k>=0} 1/((1-x^(5k+1))*(1-x^(5k+4))); a(n) =
 *            number of partitions of n into parts == 1 or 4 (mod 5), computed
 *            by the standard partition-counting DP
 * Range:     u8 n <= 36, u16 n <= 98, u32 n <= 297, u64 n <= 991,
 *            u128 n <= 3529, big any n
 * Validated: standard, pass, 77 terms through n = 9440 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static seq_t *cache = NULL;
static idx_t cache_len = 0;

seq_t a(idx_t n) {
    if (n < cache_len) return cache[n];

    idx_t len = n + 1;
    if (len < 2 * cache_len) len = 2 * cache_len;

    seq_t *dp = alloc_zero(len);
    dp[0] = 1;
    for (idx_t part = 1; part < len; part++) {
        idx_t r = part % 5;
        if (r != 1 && r != 4) continue;
        for (idx_t i = part; i < len; i++) {
            dp[i] += dp[i - part];
        }
    }

    if (cache) mem_free(cache);
    cache = dp;
    cache_len = len;
    return cache[n];
}
