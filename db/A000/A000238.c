/*
 * A000238  Number of oriented trees with n nodes.
 * https://oeis.org/A000238
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = r(n) - Sum_{i=1..n-1} r(i)*r(n-i), where r = A000151
 *            (rooted oriented trees): r(1) = 1, r(m+1) = (1/m) * Sum_{k=1..m}
 *            s(k)*r(m-k+1), s(k) = Sum_{d|k} 2*d*r(d). (G.f. A(x) = R(x) -
 *            R(x)^2.) Fixed-width builds compute modulo three primes and
 *            reconstruct by CRT (Garner) modulo 2^w; arbitrary-...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 17, u64 n <= 31, u128 n <= 57,
 *            big any n
 * Helpers:   a238_inv, a238_is_fixed, a238_mul, a238_pow, u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/u128_to_seq.h"
#include "../utils/a238_inv.h"
#include "../utils/a238_is_fixed.h"
#include "../utils/a238_mul.h"
#include "../utils/a238_pow.h"

/* ---------- exact path (arbitrary precision) ---------- */
#define A238_CAP 20005
static seq_t a238_R[A238_CAP];
static seq_t a238_S[A238_CAP];
static idx_t a238_bn = 0;

static seq_t a238_big(idx_t n)
{
    idx_t m, d, k, i;
    seq_t res;
    if (a238_bn == 0) { a238_R[1] = 1; a238_bn = 1; }
    while (a238_bn < n) {
        seq_t s = 0, t = 0;
        m = a238_bn;
        for (d = 1; d * d <= m; d++) {
            if (m % d == 0) {
                idx_t e = m / d;
                s += (seq_t)d * a238_R[d];
                if (e != d) s += (seq_t)e * a238_R[e];
            }
        }
        a238_S[m] = s * 2;
        for (k = 1; k <= m; k++) t += a238_S[k] * a238_R[m - k + 1];
        a238_R[m + 1] = t / (seq_t)m;
        a238_bn = m + 1;
    }
    res = a238_R[n];
    for (i = 1; 2 * i < n; i++) res -= 2 * (a238_R[i] * a238_R[n - i]);
    if (n % 2 == 0) res -= a238_R[n / 2] * a238_R[n / 2];
    return res;
}

/* ---------- modular path (fixed width) ---------- */
static const uint64_t a238_P[3] = {
    2305843009213693951ULL, 1000000000000000003ULL, 1000000000000000009ULL
};
static uint64_t *a238_MR[3], *a238_MS[3];
static idx_t a238_mn = 0, a238_mcap = 0;

static void a238_mod_extend(idx_t n)
{
    int j;
    if (n + 2 > a238_mcap) {
        idx_t nc = a238_mcap ? a238_mcap : 64;
        while (nc < n + 2) nc *= 2;
        for (j = 0; j < 3; j++) {
            a238_MR[j] = (uint64_t *)realloc(a238_MR[j], nc * sizeof(uint64_t));
            a238_MS[j] = (uint64_t *)realloc(a238_MS[j], nc * sizeof(uint64_t));
        }
        a238_mcap = nc;
    }
    if (a238_mn == 0) {
        for (j = 0; j < 3; j++) a238_MR[j][1] = 1;
        a238_mn = 1;
    }
    while (a238_mn < n) {
        idx_t m = a238_mn, d, k;
        for (j = 0; j < 3; j++) {
            uint64_t p = a238_P[j], s = 0, t = 0;
            uint64_t *R = a238_MR[j], *S = a238_MS[j];
            for (d = 1; d * d <= m; d++) {
                if (m % d == 0) {
                    idx_t e = m / d;
                    s = (s + a238_mul(d % p, R[d], p)) % p;
                    if (e != d) s = (s + a238_mul(e % p, R[e], p)) % p;
                }
            }
            S[m] = (s * 2) % p;
            for (k = 1; k <= m; k++) t = (t + a238_mul(S[k], R[m - k + 1], p)) % p;
            R[m + 1] = a238_mul(t, a238_inv(m, p), p);
        }
        a238_mn = m + 1;
    }
}

static seq_t a238_fixed(idx_t n)
{
    uint64_t x[3];
    int j;
    idx_t i;
    uint64_t p1 = a238_P[0], p2 = a238_P[1], p3 = a238_P[2];
    uint64_t y1, y2, y3, t;
    unsigned __int128 res;
    a238_mod_extend(n);
    for (j = 0; j < 3; j++) {
        uint64_t p = a238_P[j];
        uint64_t *R = a238_MR[j];
        uint64_t sum = 0;
        for (i = 1; i < n; i++) sum = (sum + a238_mul(R[i], R[n - i], p)) % p;
        x[j] = (R[n] + p - sum) % p;
    }
    /* Garner */
    y1 = x[0];
    y2 = a238_mul((x[1] + p2 - y1 % p2) % p2, a238_inv(p1 % p2, p2), p2);
    t = (y1 % p3 + a238_mul(p1 % p3, y2 % p3, p3)) % p3;
    y3 = a238_mul((x[2] + p3 - t) % p3,
                  a238_inv(a238_mul(p1 % p3, p2 % p3, p3), p3), p3);
    res = (unsigned __int128)y1
        + (unsigned __int128)p1 * y2
        + (unsigned __int128)p1 * p2 * y3;
    return u128_to_seq(res);
}

seq_t a(idx_t n)
{
    if (n < 1) return 0;
    if (a238_is_fixed()) return a238_fixed(n);
    return a238_big(n);
}
