/*
 * A002862  Number of nonisomorphic connected functions with no fixed points,
 *          or proper rings with n edges.
 * https://oeis.org/A002862
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = A002861(n) - A000081(n), where with T(x) = Sum r(k) x^k
 *            the rooted-tree g.f. (A000081: r(1) = 1, r(m+1) = (1/m)
 *            Sum_{k=1..m} (Sum_{d|k} d*r(d)) r(m-k+1)), Q(x) = 1/(1-T(x)),
 *            B(x) = x T'(x) Q(x), and A002861(n) = (1/n) Sum_{k|n} phi(k)
 *            B_{n/k}. Fixed-width builds compute modulo several prim...
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 45, u128 n <= 86,
 *            big any n
 * Helpers:   a001028_grow, a001028_z, a2862_fixed, a2862_mm, a2862_phi,
 *            a2862_pw
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/a001028_grow.h"
#include "../utils/a001028_z.h"
#include "../utils/a2862_fixed.h"
#include "../utils/a2862_mm.h"
#include "../utils/a2862_phi.h"
#include "../utils/a2862_pw.h"

#define A2862_MAXF 200

static uint64_t a2862_modval(idx_t n, uint64_t p)
{
    uint64_t r[A2862_MAXF + 2], s[A2862_MAXF + 2], Q[A2862_MAXF + 2], B[A2862_MAXF + 2];
    idx_t m, k, d;
    uint64_t S = 0, A;
    for (m = 0; m <= n; m++) { r[m] = 0; s[m] = 0; Q[m] = 0; B[m] = 0; }
    for (m = 1; m <= n; m++) {
        if (m == 1) {
            r[1] = 1;
        } else {
            uint64_t t = 0;
            for (k = 1; k <= m - 1; k++) t = (t + a2862_mm(s[k], r[m - k], p)) % p;
            r[m] = a2862_mm(t, a2862_pw((uint64_t)(m - 1), p - 2, p), p);
        }
        {
            uint64_t t = 0;
            for (d = 1; d <= m; d++)
                if (m % d == 0) t = (t + a2862_mm((uint64_t)d % p, r[d], p)) % p;
            s[m] = t;
        }
    }
    Q[0] = 1;
    for (m = 1; m <= n; m++) {
        uint64_t tq = 0, tb = 0;
        for (k = 1; k <= m; k++) {
            uint64_t rq = a2862_mm(r[k], Q[m - k], p);
            tq = (tq + rq) % p;
            tb = (tb + a2862_mm((uint64_t)k % p, rq, p)) % p;
        }
        Q[m] = tq;
        B[m] = tb;
    }
    for (k = 1; k <= n; k++)
        if (n % k == 0) S = (S + a2862_mm(a2862_phi(k) % p, B[n / k], p)) % p;
    A = a2862_mm(S, a2862_pw((uint64_t)n % p, p - 2, p), p);
    return (A + p - r[n]) % p;
}

static seq_t *a2862_R = NULL, *a2862_S = NULL, *a2862_Q = NULL, *a2862_B = NULL;
static idx_t a2862_have = 0, a2862_cap = 0;

static void a2862_ensure(idx_t n)
{
    idx_t m, k, d;
    if (n + 1 > a2862_cap) {
        idx_t nc = a2862_cap * 2;
        if (nc < n + 1) nc = n + 1;
        if (nc < 64) nc = 64;
        if (a2862_cap == 0) {
            a2862_R = a001028_z(nc);
            a2862_S = a001028_z(nc);
            a2862_Q = a001028_z(nc);
            a2862_B = a001028_z(nc);
            a2862_Q[0] = 1;
        } else {
            a2862_R = a001028_grow(a2862_R, a2862_cap, nc);
            a2862_S = a001028_grow(a2862_S, a2862_cap, nc);
            a2862_Q = a001028_grow(a2862_Q, a2862_cap, nc);
            a2862_B = a001028_grow(a2862_B, a2862_cap, nc);
        }
        a2862_cap = nc;
    }
    for (m = a2862_have + 1; m <= n; m++) {
        if (m == 1) {
            a2862_R[1] = 1;
        } else {
            seq_t t = 0;
            for (k = 1; k <= m - 1; k++) t += a2862_S[k] * a2862_R[m - k];
            a2862_R[m] = t / (m - 1);
        }
        {
            seq_t t = 0;
            for (d = 1; d <= m; d++)
                if (m % d == 0) t += a2862_R[d] * d;
            a2862_S[m] = t;
        }
        {
            seq_t tq = 0, tb = 0;
            for (k = 1; k <= m; k++) {
                seq_t rq = a2862_R[k] * a2862_Q[m - k];
                tq += rq;
                tb += rq * k;
            }
            a2862_Q[m] = tq;
            a2862_B[m] = tb;
        }
    }
    if (n > a2862_have) a2862_have = n;
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (a2862_fixed()) {
        static const uint64_t P[5] = {2305843009213693951ULL, 2147483647ULL,
                                      1000000007ULL, 1000000009ULL, 998244353ULL};
        uint64_t c[5];
        int i, j;
        seq_t res = 0, M = 1;
        if (n > A2862_MAXF) return 0; /* value cannot fit in <= 128 bits */
        for (i = 0; i < 5; i++) {
            uint64_t p = P[i];
            uint64_t v = a2862_modval(n, p);
            uint64_t cur = 0, mult = 1;
            for (j = 0; j < i; j++) {
                cur = (cur + a2862_mm(c[j] % p, mult, p)) % p;
                mult = a2862_mm(mult, P[j] % p, p);
            }
            c[i] = a2862_mm((v + p - cur) % p, a2862_pw(mult, p - 2, p), p);
        }
        for (i = 0; i < 5; i++) {
            res += M * (seq_t)c[i];
            M *= (seq_t)P[i];
        }
        return res;
    } else {
        seq_t S = 0;
        idx_t k;
        a2862_ensure(n);
        for (k = 1; k <= n; k++)
            if (n % k == 0) S += a2862_B[n / k] * (seq_t)a2862_phi(k);
        return S / n - a2862_R[n];
    }
}
