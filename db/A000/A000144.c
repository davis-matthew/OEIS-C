/*
 * A000144  Number of ways of writing n as a sum of 10 squares.
 * https://oeis.org/A000144
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Sum_{k=0..n} r5(k)*r5(n-k), where r5 = coefficients of
 *            theta(q)^5, theta(q) = 1 + 2*Sum_{k>=1} q^(k^2); r5 is built by
 *            repeated sparse convolution with theta
 * Range:     u8 n <= 2, u16 n <= 8, u32 n <= 135, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

static uint64_t *r5_cache = 0;
static idx_t r5_len = 0;

static void build_r5(idx_t N)
{
    idx_t m, k;
    int step;
    uint64_t *cur = (uint64_t *)malloc((size_t)(N + 1) * sizeof(uint64_t));
    uint64_t *nxt = (uint64_t *)malloc((size_t)(N + 1) * sizeof(uint64_t));
    for (m = 0; m <= N; m++) cur[m] = 0;
    cur[0] = 1;
    for (k = 1; k * k <= N; k++) cur[k * k] = 2;
    for (step = 0; step < 4; step++) {
        for (m = 0; m <= N; m++) {
            uint64_t s = cur[m];
            for (k = 1; k * k <= m; k++) s += 2 * cur[m - k * k];
            nxt[m] = s;
        }
        { uint64_t *t = cur; cur = nxt; nxt = t; }
    }
    free(nxt);
    free(r5_cache);
    r5_cache = cur;
    r5_len = N + 1;
}

seq_t a(idx_t n)
{
    idx_t k;
    seq_t s = 0;
    if (n + 1 > r5_len) {
        idx_t N = n < 64 ? 64 : n;
        if (r5_len > 0 && N < 2 * (r5_len - 1)) N = 2 * (r5_len - 1);
        build_r5(N);
    }
    for (k = 0; k <= n; k++) {
        seq_t x = (seq_t)r5_cache[k];
        seq_t y = (seq_t)r5_cache[n - k];
        s += x * y;
    }
    return s;
}
