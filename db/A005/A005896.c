/*
 * A005896  Weighted count of partitions with odd parts.
 * https://oeis.org/A005896
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   G.f.: Sum_{k>=0} ( S(q) - 1/((1-q)(1-q^3)...(1-q^(2k+1))) ),
 *            S(q) = g.f. of A000009; so a(n) = Sum_{k>=0} ( A000009(n) -
 *            #partitions of n into odd parts <= 2k+1 )
 * Range:     u8 n <= 20, u16 n <= 56, u32 n <= 186, u64 n <= 300,
 *            u128 n <= 300, big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void build(idx_t N) {
    seq_t *Q = alloc_zero(N + 1);
    seq_t *p = alloc_zero(N + 1);
    seq_t *acc = alloc_zero(N + 1);
    idx_t k, i, odd;

    /* Q = partitions into distinct parts (A000009) */
    Q[0] = 1;
    for (k = 1; k <= N; k++) {
        for (i = N; i >= k; i--) {
            Q[i] += Q[i - k];
        }
    }

    /* p = partitions into odd parts <= odd, built incrementally */
    p[0] = 1;
    for (odd = 1; odd <= N; odd += 2) {
        for (i = odd; i <= N; i++) {
            p[i] += p[i - odd];
        }
        /* for i <= odd the difference is zero */
        for (i = odd + 1; i <= N; i++) {
            acc[i] += Q[i] - p[i];
        }
    }

    release(p, N + 1);
    release(Q, N + 1);
    if (cache != NULL) release(cache, cache_len);
    cache = acc;
    cache_len = N + 1;
}

seq_t a(idx_t n) {
    if (n >= cache_len) {
        idx_t N = n;
        if (cache_len > 0 && N < 2 * cache_len) N = 2 * cache_len;
        if (N < 64) N = 64;
        build(N);
    }
    return cache[n];
}
