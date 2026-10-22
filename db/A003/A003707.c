/*
 * A003707  Expansion of e.g.f. log(1 + tan(x)).
 * https://oeis.org/A003707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   with u = 1 + tan(x), d/dx = (u^2 - 2u + 2) d/du, so f'(x) = (1 +
 *            tan^2 x)/(1 + tan x) = u - 2 + 2/u, and d/dx u^k = k*u^(k+1) -
 *            2k*u^k + 2k*u^(k-1). a(n) = f^(n)(0) = sum of the coefficients
 *            of the Laurent polynomial in u obtained after n-1 derivatives of
 *            f' (u = 1 at x = 0), a(0) = 0
 * Range:     i8 n <= 5, i16 n <= 7, i32 n <= 12, i64 n <= 19, i128 n <= 32,
 *            big any n
 * Helpers:   alloc_zero, from_i128, release, seq_is_fixed_width
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/seq_is_fixed_width.h"
#include "../utils/from_i128.h"
#include "../utils/alloc_zero.h"
#include "../utils/release.h"

static seq_t a_fixed(idx_t n) {
    size_t w = (size_t)(2 * n + 3);
    long long off = (long long)n + 1;
    unsigned __int128 *c = (unsigned __int128 *)calloc(w, sizeof(unsigned __int128));
    unsigned __int128 *d = (unsigned __int128 *)calloc(w, sizeof(unsigned __int128));
    unsigned __int128 res = 0;
    idx_t s;
    size_t i;
    c[off + 1] = 1;
    c[off] = (unsigned __int128)(__int128)(-2);
    c[off - 1] = 2;
    for (s = 1; s < n; s++) {
        unsigned __int128 *t;
        for (i = 0; i < w; i++) d[i] = 0;
        for (i = 1; i + 1 < w; i++) {
            long long e = (long long)i - off;
            unsigned __int128 v;
            if (e == 0 || c[i] == 0) continue;
            v = (unsigned __int128)(__int128)e * c[i];
            d[i + 1] += v;
            d[i] -= 2 * v;
            d[i - 1] += 2 * v;
        }
        t = c; c = d; d = t;
    }
    for (i = 0; i < w; i++) res += c[i];
    free(c);
    free(d);
    return from_i128((__int128)res);
}

static seq_t a_big(idx_t n) {
    idx_t w = 2 * n + 3;
    long long off = (long long)n + 1;
    seq_t *c = alloc_zero(w);
    seq_t *d = alloc_zero(w);
    seq_t res = 0;
    idx_t s, i;
    c[off + 1] = 1;
    c[off] = -2;
    c[off - 1] = 2;
    for (s = 1; s < n; s++) {
        seq_t *t;
        for (i = 0; i < w; i++) d[i] = 0;
        for (i = 1; i + 1 < w; i++) {
            long long e = (long long)i - off;
            seq_t v;
            if (e == 0 || c[i] == 0) continue;
            v = c[i] * (seq_t)e;
            d[i + 1] += v;
            d[i] -= v * 2;
            d[i - 1] += v * 2;
        }
        t = c; c = d; d = t;
    }
    for (i = 0; i < w; i++) res += c[i];
    release(c, w);
    release(d, w);
    return res;
}

seq_t a(idx_t n) {
    if (n == 0) return 0;
    if (seq_is_fixed_width()) return a_fixed(n);
    return a_big(n);
}
