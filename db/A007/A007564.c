/*
 * A007564  Shifts left when INVERT transform applied thrice.
 * https://oeis.org/A007564
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, eigen
 * Formula:   a(n) = (4*(2n-1)*a(n-1) - 4*(n-2)*a(n-2)) / (n+1) for n>=2, a(0)
 *            = a(1) = 1. Each step divides by n+1 exactly. To keep
 *            intermediate values small, a(n-1) and a(n-2) are split into
 *            quotient and remainder by n+1 first
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 25, u128 n <= 47,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *cache = NULL;
static idx_t cache_len = 0;
static idx_t max_computed = 1;

seq_t a(idx_t n) {
    if (n == 0 || n == 1) return 1;

    if (cache == NULL) {
        cache_len = (n + 10 < 100) ? 100 : n + 10;
        cache = (seq_t *)malloc(cache_len * sizeof(seq_t));
        cache[0] = 1;
        cache[1] = 1;
    } else if (cache_len <= n) {
        idx_t newlen = cache_len;
        while (newlen <= n) newlen *= 2;
        seq_t *nc = (seq_t *)malloc(newlen * sizeof(seq_t));
        for (idx_t j = 0; j <= max_computed; j++) nc[j] = cache[j];
        free(cache);
        cache = nc;
        cache_len = newlen;
    }

    for (idx_t i = max_computed + 1; i <= n; i++) {
        idx_t d = i + 1;
        seq_t D = (seq_t)d;
        if (D == 0) { cache[i] = 0; continue; } /* narrow type: value cannot fit anyway */
        seq_t a1 = cache[i - 1];
        seq_t a2 = cache[i - 2];
        seq_t q1 = a1 / D, q2 = a2 / D;
        long long r1 = (long long)(a1 % D);
        long long r2 = (long long)(a2 % D);
        long long R = 4LL * (long long)(2 * i - 1) * r1 - 4LL * (long long)(i - 2) * r2;
        seq_t val = (seq_t)4 * (seq_t)(2 * i - 1) * q1;
        if (R >= 0) val += (seq_t)(idx_t)(R / (long long)d);
        val -= (seq_t)4 * (seq_t)(i - 2) * q2;
        if (R < 0) val -= (seq_t)(idx_t)((-R) / (long long)d);
        cache[i] = val;
    }
    if (n > max_computed) max_computed = n;

    return cache[n];
}
