/*
 * A007837  Number of partitions of n-set with distinct block sizes.
 * https://oeis.org/A007837
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = number of set partitions of an n-set with distinct block
 *            sizes. DP over block size m = 1..N: f(i) += binomial(i,m) *
 *            f(i-m) (i descending), f(0) = 1, which is the coefficient
 *            extraction of E.g.f. Product_{m>=1} (1+x^m/m!)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 17, u64 n <= 29, u128 n <= 49,
 *            big any n
 * Validated: standard, pass, 44 terms through n = 654 (2026-11-03)
 * Helpers:   gcd_u64, mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static seq_t *A7837_cache = NULL;
static idx_t A7837_len = 0; /* number of valid entries: indices 0..len-1 */

static void A7837_build(idx_t N) {
    seq_t *f = alloc_zero(N + 1);
    seq_t *c = alloc_zero(N + 1);
    idx_t i, m;
    f[0] = 1;
    for (m = 1; m <= N; m++) {
        /* c[i] = binomial(i, m) for i = m..N, computed ascending */
        c[m] = 1;
        for (i = m + 1; i <= N; i++) {
            /* C(i,m) = C(i-1,m) * i / (i-m) */
            uint64_t d = i - m;
            uint64_t g = gcd_u64(i, d);
            seq_t t = c[i - 1] / (seq_t)(d / g);
            c[i] = t * (seq_t)(i / g);
        }
        for (i = N; i >= m; i--) {
            if (f[i - m] != 0)
                f[i] += c[i] * f[i - m];
            if (i == m) break;
        }
    }
    release(c, N + 1);
    if (A7837_cache) release(A7837_cache, A7837_len);
    A7837_cache = f;
    A7837_len = N + 1;
}

seq_t a(idx_t n) {
    if (n >= A7837_len) {
        idx_t N = n;
        idx_t grow = A7837_len + A7837_len / 2;
        if (grow > N) N = grow;
        if (N < 16) N = 16;
        A7837_build(N);
    }
    return A7837_cache[n];
}
