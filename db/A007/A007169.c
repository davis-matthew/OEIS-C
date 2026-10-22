/*
 * A007169  Number of Q-graphs rooted at a polygon.
 * https://oeis.org/A007169
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   G.f. Sum_{r>=2} Z(C_{2r}; B(x)), where B(x) = x^(1/2)*C(x) and C
 *            is the g.f. of A003168. Rewriting the sum with logarithms and
 *            using Lagrange inversion gives D = C-1 = x*(1+D)^2*(1+2D) and
 *            B(t) = t*(1-B^2)/(1-2B^2). Then 2n*a(n) = Sum_{d|n, d odd}
 *            phi(d)*P1(n/d) + Sum_{e|n} phi(2e)*P2(n/e) - Q1(n...
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   pw2, seq_is_big, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_is_big.h"
#include "../utils/pw2.h"
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static uint64_t tot(uint64_t x)
{
    uint64_t r = x;
    for (uint64_t p = 2; p * p <= x; p++)
        if (x % p == 0) {
            while (x % p == 0) x /= p;
            r -= r / p;
        }
    if (x > 1) r -= r / x;
    return r;
}

/* H(p,q,r,plus) = Sum_{i=0..min(p,r)} C(p,i) * 2^(plus ? i : p-i) * C(q,r-i), r <= q */
static seq_t Hbig(idx_t p, idx_t q, idx_t r, int plus)
{
    seq_t t = 1;
    if (!plus) t = pw2(p);
    for (idx_t k = 0; k < r; k++) { t *= (idx_t)(q - k); t /= (idx_t)(k + 1); }
    seq_t s = 0;
    idx_t lim = p < r ? p : r;
    for (idx_t i = 0;; i++) {
        s += t;
        if (i == lim) break;
        t *= (idx_t)((p - i) * (r - i) * (plus ? 2 : 1));
        t /= (idx_t)((i + 1) * (q - r + i + 1) * (plus ? 1 : 2));
    }
    return s;
}

/* same sum, wrapping mod 2^128 (binomials themselves exact for small n) */
static u128 Hfix(idx_t p, idx_t q, idx_t r, int plus)
{
    u128 cq = 1, cp = 1, s = 0;
    for (idx_t k = 0; k < r; k++) cq = cq * (u128)(q - k) / (u128)(k + 1);
    idx_t lim = p < r ? p : r;
    for (idx_t i = 0;; i++) {
        idx_t e = plus ? i : p - i;
        u128 pw = e < 128 ? ((u128)1 << e) : (u128)0;
        s += cp * pw * cq;
        if (i == lim) break;
        cp = cp * (u128)(p - i) / (u128)(i + 1);
        cq = cq * (u128)(r - i) / (u128)(q - r + i + 1);
    }
    return s;
}

seq_t a(idx_t n)
{
    if (n < 2) return 0;
    if (seq_is_big()) {
        seq_t N = 0;
        for (idx_t d = 1; d <= n; d++) {
            if (n % d) continue;
            idx_t m = n / d;
            if (d & 1) N += Hbig(m - 1, 2 * m - 1, m - 1, 1) * (idx_t)tot(d);
            N += Hbig((m - 1) / 2, m - 1, m - 1, 0) * (idx_t)tot(2 * d);
        }
        N -= Hbig(n - 2, 2 * n, n - 1, 1);
        if (n & 1) N -= Hbig((n - 1) / 2 - 1, n, n - 1, 0);
        return N / (idx_t)(2 * n);
    } else {
        u128 N = 0;
        for (idx_t d = 1; d <= n; d++) {
            if (n % d) continue;
            idx_t m = n / d;
            if (d & 1) N += Hfix(m - 1, 2 * m - 1, m - 1, 1) * (u128)tot(d);
            N += Hfix((m - 1) / 2, m - 1, m - 1, 0) * (u128)tot(2 * d);
        }
        N -= Hfix(n - 2, 2 * n, n - 1, 1);
        if (n & 1) N -= Hfix((n - 1) / 2 - 1, n, n - 1, 0);
        u128 o = (u128)(2 * n);
        while (!(o & 1)) { o >>= 1; N >>= 1; }
        u128 x = o;
        for (int k = 0; k < 7; k++) x *= (u128)2 - o * x;
        N *= x;
        return u128_to_seq(N);
    }
}
