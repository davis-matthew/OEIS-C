/*
 * A005036  Number of nonequivalent dissections of a polygon into n
 *          quadrilaterals by nonintersecting diagonals up to rotation and
 *          reflection.
 * https://oeis.org/A005036
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  core, nonn, nice
 * Formula:   a(n) = (2*A001764(n)/(n+1) + 6*[n even]*A001764(n/2) + 5*[n
 *            odd]*A006013((n-1)/2) + 2*[n == 1 mod 4]*A001764((n-1)/4)) / 8,
 *            i.e. a(n) = (2*(3n)!/((2n+1)!(n+1)!) + 6*[n
 *            even]*(3k)!/(k!(2k+1)!) |k=n/2 + 5*[n
 *            odd]*(3k+1)!/((2k+1)!(k+1)!) |k=(n-1)/2 + 2*[n==1 mod
 *            4]*(3m)!/(m!(2m+1)!) |m=(n-1)/4) / 8,...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 29, u128 n <= 53,
 *            big any n
 * Validated: standard, pass, 46 terms through n = 98 (2026-11-01)
 * Helpers:   valuation
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/valuation.h"

static uint64_t vp_fact(uint64_t N, uint64_t p) {
    uint64_t e = 0;
    while (N >= p) { N /= p; e += N; }
    return e;
}

/* value = c * A! / (B! * C!) written as 8*q + low, low < 8 */
static void term8(uint64_t c, uint64_t A, uint64_t B, uint64_t C,
                  const unsigned char *comp, seq_t *qo, uint64_t *lowo) {
    uint64_t lim = A > 6 ? A : 6;
    uint64_t e2 = vp_fact(A, 2) + valuation(c, 2) - vp_fact(B, 2) - vp_fact(C, 2);
    uint64_t s = e2 < 3 ? e2 : 3;
    uint64_t D = (uint64_t)8 >> s;
    seq_t q;
    uint64_t r;
    if (D == 1) { q = 1; r = 0; } else { q = 0; r = 1; }
    uint64_t f = 1;
    for (uint64_t p = 2; p <= lim; p++) {
        if (comp[p]) continue;
        uint64_t e;
        if (p == 2) e = e2 - s;
        else e = vp_fact(A, p) + valuation(c, p) - vp_fact(B, p) - vp_fact(C, p);
        for (uint64_t i = 0; i < e; i++) {
            if (f * p > ((uint64_t)1 << 24)) {
                q = q * (idx_t)f + (seq_t)((r * f) / D);
                r = (r * f) % D;
                f = 1;
            }
            f *= p;
        }
    }
    if (f > 1) {
        q = q * (idx_t)f + (seq_t)((r * f) / D);
        r = (r * f) % D;
    }
    *qo = q;
    *lowo = r << s;
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    uint64_t lim = 3 * (uint64_t)n + 2;
    if (lim < 7) lim = 7;
    unsigned char *comp = (unsigned char *)malloc((size_t)(lim + 1));
    memset(comp, 0, (size_t)(lim + 1));
    comp[0] = comp[1] = 1;
    for (uint64_t i = 2; i * i <= lim; i++)
        if (!comp[i])
            for (uint64_t j = i * i; j <= lim; j += i) comp[j] = 1;

    seq_t total = 0, q;
    uint64_t low = 0, l;

    term8(2, 3 * n, 2 * n + 1, n + 1, comp, &q, &l);
    total += q; low += l;

    if (n % 2 == 0) {
        uint64_t k = n / 2;
        term8(6, 3 * k, k, 2 * k + 1, comp, &q, &l);
        total += q; low += l;
    } else {
        uint64_t k = (n - 1) / 2;
        term8(5, 3 * k + 1, 2 * k + 1, k + 1, comp, &q, &l);
        total += q; low += l;
    }
    if ((n - 1) % 4 == 0) {
        uint64_t m = (n - 1) / 4;
        term8(2, 3 * m, m, 2 * m + 1, comp, &q, &l);
        total += q; low += l;
    }
    free(comp);
    total += (seq_t)(low / 8);
    return total;
}
