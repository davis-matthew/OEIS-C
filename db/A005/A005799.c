/*
 * A005799  Generalized Euler numbers of type 2^n.
 * https://oeis.org/A005799
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   G.f.: A(x) = 1/G(0) where G(k) = 1 - x*(k+1)*(2*k+1)/(1 -
 *            x*(k+1)*(2*k+1)/G(k+1)), i.e. an S-fraction with coefficients
 *            alpha(2k+1) = alpha(2k+2) = (k+1)*(2k+1). So a(n) = weighted
 *            count of Dyck paths of length 2n in which a down step from
 *            height h has weight alpha(h); a(0) = 1. Computed by DP ov...
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 8, u64 n <= 13, u128 n <= 16,
 *            big any n
 * Helpers:   alloc_zero, release
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t *cache = NULL;
static idx_t clen = 0;

static seq_t alpha(idx_t h) {
    idx_t k = (h - 1) / 2;
    return ((seq_t)k + 1) * ((seq_t)(2 * k) + 1);
}

seq_t a(idx_t n) {
    if (n >= clen) {
        idx_t N = n + 8;
        seq_t *v = alloc_zero(N + 2);
        seq_t *res = alloc_zero(N + 1);
        idx_t t, h;
        v[0] = 1;
        res[0] = 1;
        for (t = 1; t <= 2 * N; t++) {
            idx_t lim = (t < 2 * N - t) ? t : 2 * N - t;
            for (h = (t & 1); h <= lim; h += 2) {
                if (h == 0)
                    v[0] = alpha(1) * v[1];
                else
                    v[h] = v[h - 1] + alpha(h + 1) * v[h + 1];
            }
            if ((t & 1) == 0) res[t / 2] = v[0];
        }
        release(v, N + 2);
        if (cache) release(cache, clen);
        cache = res;
        clen = N + 1;
    }
    return cache[n];
}
