/*
 * A233420  Minimal number of c-squares (A020330) and/or 1's which add to n.
 * https://oeis.org/A233420
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = minimum number of terms from {1} union A020330 summing to
 *            n; computed by coin-change DP over the parts
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Validated: partial, pass, 84 terms through n = 84 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    size_t cap = 64;
    size_t cnt = 0;
    uint64_t *parts = (uint64_t *)malloc(cap * sizeof(uint64_t));
    unsigned k;
    for (k = 1; k < 32; k++) {
        uint64_t lo = (uint64_t)1 << (k - 1);
        uint64_t m = ((uint64_t)1 << k) + 1;
        if (lo * m > n) break;
        for (uint64_t s = lo; s < ((uint64_t)1 << k); s++) {
            uint64_t v = s * m;
            if (v > n) break;
            if (cnt == cap) {
                cap *= 2;
                parts = (uint64_t *)realloc(parts, cap * sizeof(uint64_t));
            }
            parts[cnt++] = v;
        }
    }
    unsigned char *dp = (unsigned char *)malloc((size_t)n + 1);
    dp[0] = 0;
    for (uint64_t i = 1; i <= n; i++) {
        unsigned best = (unsigned)dp[i - 1] + 1;
        for (size_t j = 0; j < cnt; j++) {
            uint64_t p = parts[j];
            if (p > i) break;
            unsigned c = (unsigned)dp[i - p] + 1;
            if (c < best) best = c;
        }
        dp[i] = (unsigned char)best;
    }
    seq_t r = (seq_t)dp[n];
    free(dp);
    free(parts);
    return r;
}
