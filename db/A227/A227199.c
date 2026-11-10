/*
 * A227199  Primes that divide some term of A006720.
 * https://oeis.org/A227199
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   search over primes p != 37 (37 is the only prime of bad
 *            reduction). A006720(n) = |W(2n-3)|, where W is the elliptic
 *            divisibility sequence of P=(0,0) on E: y^2 + y = x^3 - x. So p
 *            divides some term iff P has odd order on E mod p. The order of P
 *            is found exactly by brute force for small p, and for...
 * Range:     u8 n <= 30, u16 n <= 3434, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 71 terms through n = 9715 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef struct { uint64_t x, y; int inf; } A227199_pt;

static uint64_t A227199_inv(uint64_t a, uint64_t p)
{
    int64_t t = 0, nt = 1, r = (int64_t)p, nr = (int64_t)(a % p);
    while (nr != 0) {
        int64_t q = r / nr, tmp;
        tmp = t - q * nt; t = nt; nt = tmp;
        tmp = r - q * nr; r = nr; nr = tmp;
    }
    if (t < 0) t += (int64_t)p;
    return (uint64_t)t;
}

static A227199_pt A227199_add(A227199_pt A, A227199_pt B, uint64_t p)
{
    A227199_pt R;
    uint64_t num, den, lam, nu;
    if (A.inf) return B;
    if (B.inf) return A;
    if (A.x == B.x) {
        if ((A.y + B.y + 1) % p == 0) { R.x = 0; R.y = 0; R.inf = 1; return R; }
        num = (3 * (A.x * A.x % p) + p - 1) % p;
        den = (2 * A.y + 1) % p;
    } else {
        num = (B.y + p - A.y) % p;
        den = (B.x + p - A.x) % p;
    }
    lam = num * A227199_inv(den, p) % p;
    R.x = (lam * lam % p + 2 * p - A.x - B.x) % p;
    nu = (A.y + p - lam * A.x % p) % p;
    R.y = (3 * p - lam * R.x % p - nu - 1) % p;
    R.inf = 0;
    return R;
}

static A227199_pt A227199_mul(uint64_t k, A227199_pt P, uint64_t p)
{
    A227199_pt R, Q = P;
    R.x = 0; R.y = 0; R.inf = 1;
    while (k) {
        if (k & 1) R = A227199_add(R, Q, p);
        k >>= 1;
        if (k) Q = A227199_add(Q, Q, p);
    }
    return R;
}

static int A227199_eq(A227199_pt A, A227199_pt B)
{
    if (A.inf || B.inf) return A.inf && B.inf;
    return A.x == B.x && A.y == B.y;
}

static int A227199_member(uint64_t p)
{
    A227199_pt P, R;
    P.x = 0; P.y = 0; P.inf = 0;
    if (p == 37) return 0;
    if (p < 200) {
        uint64_t ord = 1;
        R = P;
        while (!R.inf) { R = A227199_add(R, P, p); ord++; }
        return (ord & 1) != 0;
    } else {
        static A227199_pt baby[2048];
        uint64_t s = 0, W, lo, m, j, i, N = 0;
        A227199_pt G;
        while ((s + 1) * (s + 1) <= p) s++;
        W = 4 * s + 4;
        lo = p + 1 - (2 * s + 2);
        m = 0;
        while (m * m < W) m++;
        m++;
        baby[0].x = 0; baby[0].y = 0; baby[0].inf = 1;
        for (j = 1; j <= m; j++) baby[j] = A227199_add(baby[j - 1], P, p);
        G = baby[m];
        R = A227199_mul(lo, P, p);
        for (i = 1; i <= W / m + 3 && N == 0; i++) {
            R = A227199_add(R, G, p);
            for (j = 0; j <= m; j++) {
                if (A227199_eq(R, baby[j])) { N = lo + i * m - j; break; }
            }
        }
        if (N == 0) return 0; /* should not happen */
        while ((N & 1) == 0) N >>= 1;
        R = A227199_mul(N, P, p);
        return R.inf;
    }
}

static int A227199_isprime(uint64_t c)
{
    uint64_t d;
    if (c < 2) return 0;
    if (c % 2 == 0) return c == 2;
    for (d = 3; d * d <= c; d += 2)
        if (c % d == 0) return 0;
    return 1;
}

seq_t a(idx_t n)
{
    static uint64_t *list = 0;
    static size_t cnt = 0, cap = 0;
    static uint64_t cand = 1;
    while (cnt < (size_t)n) {
        cand++;
        if (A227199_isprime(cand) && A227199_member(cand)) {
            if (cnt == cap) {
                cap = cap ? cap * 2 : 1024;
                list = (uint64_t *)realloc(list, cap * sizeof(uint64_t));
            }
            list[cnt++] = cand;
        }
    }
    return (seq_t)list[n - 1];
}
