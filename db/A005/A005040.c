/*
 * A005040  Number of nonequivalent dissections of a polygon into n pentagons
 *          by nonintersecting diagonals up to rotation and reflection.
 * https://oeis.org/A005040
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (8*g(n) - 3*h(n) + 15*[n even]*g(n/2) + 10*[n
 *            odd]*h((n-1)/2) + 8*[n == 1 mod 5]*g((n-1)/5)) / 20, where g(k)
 *            = A002293(k) = binomial(4k,k)/(3k+1) = [z^k] G(z), G = 1 +
 *            z*G^4, and h(k) = [z^k] G(z)^2 = binomial(4k+2,k)/(2k+1). Exact
 *            closed forms for big integers; for fixed widths the serie...
 * Range:     u8 n <= 6, u16 n <= 9, u32 n <= 14, u64 n <= 24, u128 n <= 45,
 *            big any n
 * Helpers:   binom, seq_is_fixed_width, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/binom.h"
#include "../utils/seq_is_fixed_width.h"
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;
typedef struct { u128 lo; unsigned hi; } w130;

static unsigned mulhi2(u128 x, u128 y) {
    u128 x0 = (uint64_t)x, x1 = x >> 64, y0 = (uint64_t)y, y1 = y >> 64;
    u128 p00 = x0 * y0, p01 = x0 * y1, p10 = x1 * y0, p11 = x1 * y1;
    u128 mid = (p00 >> 64) + (uint64_t)p01 + (uint64_t)p10;
    u128 hi = p11 + (p01 >> 64) + (p10 >> 64) + (mid >> 64);
    return (unsigned)(hi & 3);
}

static w130 w_add(w130 a, w130 b) {
    w130 r;
    r.lo = a.lo + b.lo;
    r.hi = (a.hi + b.hi + (r.lo < a.lo ? 1u : 0u)) & 3u;
    return r;
}

static w130 w_mul(w130 a, w130 b) {
    w130 r;
    r.lo = a.lo * b.lo;
    r.hi = (mulhi2(a.lo, b.lo) + a.hi * (unsigned)(b.lo & 3) + b.hi * (unsigned)(a.lo & 3)) & 3u;
    return r;
}

static w130 w_const(u128 lo, unsigned hi) {
    w130 r;
    r.lo = lo;
    r.hi = hi & 3u;
    return r;
}

static w130 *Gs = NULL, *G2s = NULL, *G4s = NULL;
static idx_t wlen = 0;

static void w_extend(idx_t n) {
    idx_t k;
    if (n < wlen) return;
    Gs = (w130 *)realloc(Gs, (size_t)(n + 1) * sizeof(w130));
    G2s = (w130 *)realloc(G2s, (size_t)(n + 1) * sizeof(w130));
    G4s = (w130 *)realloc(G4s, (size_t)(n + 1) * sizeof(w130));
    for (k = wlen; k <= n; k++) {
        idx_t j;
        w130 s;
        if (k == 0) {
            Gs[0] = G2s[0] = G4s[0] = w_const(1, 0);
            continue;
        }
        Gs[k] = G4s[k - 1];
        s = w_const(0, 0);
        for (j = 0; j <= k; j++) s = w_add(s, w_mul(Gs[j], Gs[k - j]));
        G2s[k] = s;
        s = w_const(0, 0);
        for (j = 0; j <= k; j++) s = w_add(s, w_mul(G2s[j], G2s[k - j]));
        G4s[k] = s;
    }
    wlen = n + 1;
}

static seq_t big_g(idx_t k) {
    return binom(4 * k, k) / (seq_t)(3 * k + 1);
}

static seq_t big_h(idx_t k) {
    return binom(4 * k + 2, k) / (seq_t)(2 * k + 1);
}

seq_t a(idx_t n) {
    if (!seq_is_fixed_width()) {
        seq_t N = 8 * big_g(n);
        if (n % 2 == 0) N += 15 * big_g(n / 2);
        else N += 10 * big_h((n - 1) / 2);
        if ((n - 1) % 5 == 0) N += 8 * big_g((n - 1) / 5);
        N -= 3 * big_h(n);
        return N / 20;
    } else {
        w130 N;
        u128 q, inv5;
        int i;
        if (n > 3000) return 0; /* far beyond any fixed-width range */
        w_extend(n);
        N = w_mul(w_const(8, 0), Gs[n]);
        N = w_add(N, w_mul(w_const(~(u128)0 - 2, 3), G2s[n])); /* -3 mod 2^130 */
        if (n % 2 == 0) N = w_add(N, w_mul(w_const(15, 0), Gs[n / 2]));
        else N = w_add(N, w_mul(w_const(10, 0), G2s[(n - 1) / 2]));
        if ((n - 1) % 5 == 0) N = w_add(N, w_mul(w_const(8, 0), Gs[(n - 1) / 5]));
        q = (N.lo >> 2) | ((u128)N.hi << 126);
        inv5 = 5;
        for (i = 0; i < 7; i++) inv5 = inv5 * (2 - 5 * inv5);
        return u128_to_seq(q * inv5);
    }
}
