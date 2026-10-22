/*
 * A002098  G.f.: 1/Product_{k>=1} (1-prime(k)*x^prime(k)).
 * https://oeis.org/A002098
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G.f.: 1/Product_{k>=1} (1-prime(k)*x^prime(k)); computed by the
 *            in-place DP a_p(s) = a(s) + p*a_p(s-p) over primes p <= n, a(0)
 *            = 1
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 50, u64 n <= 108,
 *            u128 n <= 228, big any n
 * Helpers:   a001028_z
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stdlib.h>
#include "../utils/a001028_z.h"

static seq_t *cache = 0;
static idx_t cache_len = 0;

static void build(idx_t len) {
    idx_t N = len - 1;
    seq_t *dp = a001028_z(len);
    char *comp = (char *)malloc((size_t)len);
    idx_t i, j, s;
    for (i = 0; i < len; i++) { dp[i] = 0; comp[i] = 0; }
    dp[0] = 1;
    for (i = 2; i <= N; i++) {
        if (comp[i]) continue;
        for (j = i * i; j <= N; j += i) comp[j] = 1;
        for (s = i; s <= N; s++)
            dp[s] += dp[s - i] * (seq_t)i;
    }
    free(comp);
    cache = dp;
    cache_len = len;
}

seq_t a(idx_t n) {
    if (n >= cache_len) {
        idx_t len = cache_len * 2;
        if (len < n + 1) len = n + 1;
        if (len < 64) len = 64;
        build(len);
    }
    return cache[n];
}
