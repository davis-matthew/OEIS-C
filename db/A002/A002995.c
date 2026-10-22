/*
 * A002995  Number of unlabeled planar trees (also called plane trees) with n
 *          nodes.
 * https://oeis.org/A002995
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = 1/(2*(n-1))*sum_{d|(n-1)} phi((n-1)/d)*binomial(2d,d) -
 *            A000108(n-1)/2 + [n even]*A000108(n/2-1)/2, i.e. a(n) = S/(2m),
 *            m = n-1, S = sum_{d|m} phi(m/d)*C(2d,d) - m*Cat(m) + [n
 *            even]*m*Cat(n/2-1). Fixed-width builds keep every value as
 *            q*(2m)+r so no intermediate overflows the exact result
 * Range:     u8 n <= 9, u16 n <= 14, u32 n <= 23, u64 n <= 40, u128 n <= 73,
 *            big any n
 * Helpers:   a2995_big, a2995_cbin, a2995_is_fixed, a2995_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2995_big.h"
#include "../utils/a2995_cbin.h"
#include "../utils/a2995_is_fixed.h"
#include "../utils/a2995_phi.h"

typedef struct { seq_t q; uint64_t r; } a2995_qr;

static a2995_qr a2995_add(a2995_qr x, a2995_qr y, uint64_t D) {
    a2995_qr s;
    s.q = x.q + y.q;
    s.r = x.r + y.r;
    if (s.r >= D) { s.r -= D; s.q += 1; }
    return s;
}

static a2995_qr a2995_sub(a2995_qr x, a2995_qr y, uint64_t D) {
    a2995_qr s;
    s.q = x.q - y.q;
    if (x.r < y.r) { s.r = x.r + D - y.r; s.q -= 1; }
    else s.r = x.r - y.r;
    return s;
}

static a2995_qr a2995_mulc(a2995_qr x, uint64_t c, uint64_t D) {
    a2995_qr s;
    uint64_t t = x.r * c;
    s.q = x.q * c + (seq_t)(t / D);
    s.r = t % D;
    return s;
}

static seq_t a2995_fixed(idx_t n) {
    idx_t m = n - 1;
    uint64_t D = 2 * (uint64_t)m;
    idx_t k = n / 2 - 1, r, j;
    int even = (n % 2 == 0);
    a2995_qr zero, S, catm, catk;
    a2995_qr *prev = (a2995_qr *)malloc((size_t)(m + 3) * sizeof(a2995_qr));
    a2995_qr *cur = (a2995_qr *)malloc((size_t)(m + 3) * sizeof(a2995_qr));
    a2995_qr *tmp;
    zero.q = 0; zero.r = 0;
    S = zero; catm = zero; catk = zero;
    prev[0].q = 0; prev[0].r = 1;
    for (r = 1; r <= 2 * m; r++) {
        idx_t Lr = r / 2 + 1, Lp = (r - 1) / 2 + 1;
        if (Lr > r) Lr = r;
        if (Lp > r - 1) Lp = r - 1;
        for (j = 0; j <= Lr; j++) {
            a2995_qr left, right;
            if (j == 0) left = zero;
            else left = (j - 1 > Lp) ? prev[r - j] : prev[j - 1];
            if (j > r - 1) right = zero;
            else right = (j > Lp) ? prev[r - 1 - j] : prev[j];
            cur[j] = a2995_add(left, right, D);
        }
        if (r % 2 == 0) {
            idx_t d = r / 2;
            if (m % d == 0)
                S = a2995_add(S, a2995_mulc(cur[d], a2995_phi(m / d), D), D);
            if (even && d == k)
                catk = a2995_sub(cur[d], cur[d + 1], D);
            if (d == m)
                catm = a2995_sub(cur[d], cur[d + 1], D);
        }
        tmp = prev; prev = cur; cur = tmp;
    }
    /* row 0 case for k == 0 (n == 2) cannot occur since n >= 4 here */
    S = a2995_sub(S, a2995_mulc(catm, m, D), D);
    if (even) S = a2995_add(S, a2995_mulc(catk, m, D), D);
    free(prev);
    free(cur);
    return S.q;
}

seq_t a(idx_t n) {
    if (n <= 3) return 1;
    if (a2995_is_fixed()) return a2995_fixed(n);
    return a2995_big(n);
}
