/*
 * A005270  Number of sequences s of length n with s[1]=1, s[2]=1,
 *          s[j-1]<s[j]<=s[j-2]+s[j-1] for j>=3.
 * https://oeis.org/A005270
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = g[n-2](1,1), where g[0](x,y) = 1 and g[k](x,y) =
 *            Sum_{z=y+1..x+y} g[k-1](y,z). g[k](x,y) is written as Sum_a
 *            binomial(x,a)*v_a(y), where each v_a is a polynomial in y kept
 *            as its values at y = 0..k+1. Then v'_{j+1}(y) = Sum_a
 *            binomial(y,a) * Delta^j v_a(y+1), with Delta the forward
 *            difference
 * Range:     u8 n <= 8, u16 n <= 10, u32 n <= 13, u64 n <= 17, u128 n <= 24,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cache = NULL;
static idx_t cache_len = 0;

static void build(idx_t N) {
    idx_t K = N - 2, W = K + 3, y, a, k, i, m, p, j;
    seq_t *C = alloc_zero(W * W);
    seq_t *v = alloc_zero(W * W);
    seq_t *nv = alloc_zero(W * W);
    seq_t *r = alloc_zero(W + 1);
    seq_t *res = alloc_zero(N + 1);
    seq_t *tmp;

    for (y = 0; y < W; y++) {
        C[y * W] = (seq_t)1;
        for (a = 1; a <= y; a++)
            C[y * W + a] = C[(y - 1) * W + a - 1] + C[(y - 1) * W + a];
    }
    v[0] = (seq_t)1;
    v[1] = (seq_t)1;
    for (k = 0; k <= K; k++) {
        res[k + 2] = v[0 * W + 1] + v[1 * W + 1];
        if (k == K) break;
        for (i = 0; i < W * W; i++) nv[i] = (seq_t)0;
        for (a = 0; a <= k; a++) {
            for (i = 0; i <= k; i++) r[i] = v[a * W + i];
            r[k + 1] = (seq_t)0;
            for (i = 1; i <= k; i++)
                for (m = k; m >= i; m--) r[m] -= r[m - 1];
            for (p = 0; p <= k + 2; p++) {
                if (p >= 1) {
                    y = p - 1;
                    if (a <= y) {
                        seq_t c = C[y * W + a];
                        for (j = 0; j <= k; j++)
                            nv[(j + 1) * W + y] += c * r[j];
                    }
                }
                for (i = 0; i <= k; i++) r[i] += r[i + 1];
            }
        }
        tmp = v; v = nv; nv = tmp;
    }
    release(C, W * W);
    release(v, W * W);
    release(nv, W * W);
    release(r, W + 1);
    if (cache) release(cache, cache_len);
    cache = res;
    cache_len = N + 1;
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0;
    if (n >= cache_len) build(n);
    return cache[n];
}
