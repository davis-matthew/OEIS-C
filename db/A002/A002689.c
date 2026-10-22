/*
 * A002689  Denominators of coefficients for repeated integration.
 * https://oeis.org/A002689
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = A002207(n)/(n+1), where A002207(n) = denominator(G(n+1)),
 *            the Gregory coefficients G(m) = (1/m!) * Sum_{k=1..m}
 *            Stirling1(m,k)/(k+1). With L = lcm(1..m+1), N = Sum_{k=1..m}
 *            Stirling1(m,k)*L/(k+1) and D = m!*L, the reduced denominator is
 *            D/gcd(N,D)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 30,
 *            big any n
 * Helpers:   a2689_flog, a2689_isp, a2689_lk, a2689_mulm, a2689_small,
 *            a2689_v, a2689_vfact
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a2689_flog.h"
#include "../utils/a2689_isp.h"
#include "../utils/a2689_lk.h"
#include "../utils/a2689_mulm.h"
#include "../utils/a2689_small.h"
#include "../utils/a2689_v.h"
#include "../utils/a2689_vfact.h"

seq_t a(idx_t n) {
    uint64_t m = (uint64_t)n + 1;
    seq_t r;
    if (a2689_small(m, &r)) return r;

    /* exact path (arbitrary precision) */
    seq_t *c = (seq_t *)calloc((size_t)(m + 1), sizeof(seq_t));
    for (uint64_t k = 0; k <= m; k++) c[k] = 0;
    c[1] = 1;
    for (uint64_t j = 1; j < m; j++)
        for (uint64_t k = j + 1; k >= 1; k--)
            c[k] = c[k - 1] + c[k] * (seq_t)j;
    seq_t L = 1;
    for (uint64_t p = 2; p <= m + 1; p++) {
        if (!a2689_isp(p)) continue;
        uint64_t f = a2689_flog(p, m + 1);
        for (uint64_t i = 0; i < f; i++) L *= (seq_t)p;
    }
    seq_t pos = 0, neg = 0;
    for (uint64_t k = 1; k <= m; k++) {
        seq_t t = c[k] * (L / (seq_t)(k + 1));
        if ((m - k) & 1) neg += t;
        else pos += t;
    }
    free(c);
    seq_t N = pos >= neg ? pos - neg : neg - pos;
    seq_t D = L;
    for (uint64_t i = 2; i <= m; i++) D *= (seq_t)i;
    seq_t x = N, y = D;
    while (y != 0) { seq_t t = x % y; x = y; y = t; }
    if (x == 0) return 0;
    return D / x / (seq_t)m;
}
