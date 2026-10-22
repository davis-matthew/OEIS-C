/*
 * A002297  Numerator of (2/Pi) * Integral_{x>=0} (sin x / x)^n dx.
 * https://oeis.org/A002297
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, frac, easy, nice
 * Formula:   a(n) = numerator((n/2^(n-1)) * Sum_{r=0..n/2}
 *            (-1)^r*(n-2*r)^(n-1)/(r!*(n-r)!)) computed as numerator of
 *            S/(2^(n-1)*(n-1)!) where S = Sum_{r=0..n/2}
 *            (-1)^r*C(n,r)*(n-2r)^(n-1), using exact internal multiprecision,
 *            reducing by the primes <= n-1
 * Range:     u8 n <= 6, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 32,
 *            big any n
 * Helpers:   q2297_add, q2297_div, q2297_legendre, q2297_mod, q2297_mul,
 *            q2297_sub, q2297_trim
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/q2297_add.h"
#include "../utils/q2297_div.h"
#include "../utils/q2297_legendre.h"
#include "../utils/q2297_mod.h"
#include "../utils/q2297_mul.h"
#include "../utils/q2297_sub.h"
#include "../utils/q2297_trim.h"

seq_t a(idx_t n) {
    size_t N = (size_t)n;
    size_t bl = 0, t = N;
    while (t) { bl++; t >>= 1; }
    size_t cap = (N + N * bl) / 64 + 6;
    uint64_t *C = (uint64_t *)calloc(cap, sizeof(uint64_t));
    uint64_t *T = (uint64_t *)calloc(cap, sizeof(uint64_t));
    uint64_t *P = (uint64_t *)calloc(cap, sizeof(uint64_t));
    uint64_t *Q = (uint64_t *)calloc(cap, sizeof(uint64_t));
    size_t cl = 1, tl, pl = 0, ql = 0;
    size_t r, i;
    C[0] = 1;
    for (r = 0; r <= N / 2; r++) {
        if (r > 0) {
            q2297_mul(C, &cl, (uint64_t)(N - r + 1));
            q2297_div(C, &cl, (uint64_t)r);
        }
        uint64_t m = (uint64_t)(N - 2 * r);
        uint64_t e = (uint64_t)(N - 1);
        if (m == 0 && e > 0) continue;
        memcpy(T, C, cl * sizeof(uint64_t));
        tl = cl;
        if (m > 1) {
            while (e > 0) {
                uint64_t mk = m, k = 1;
                while (k < e && mk <= UINT64_MAX / m) { mk *= m; k++; }
                q2297_mul(T, &tl, mk);
                e -= k;
            }
        }
        if (r & 1) q2297_add(Q, &ql, T, tl);
        else q2297_add(P, &pl, T, tl);
    }
    q2297_sub(P, &pl, Q, ql);

    /* remove powers of 2 */
    {
        uint64_t e2 = (uint64_t)(N - 1) + q2297_legendre((uint64_t)(N - 1), 2);
        uint64_t tz = 0;
        for (i = 0; i < pl; i++) {
            if (P[i] == 0) { tz += 64; continue; }
            uint64_t w = P[i];
            while (!(w & 1)) { w >>= 1; tz++; }
            break;
        }
        uint64_t s = tz < e2 ? tz : e2;
        size_t qs = (size_t)(s / 64);
        unsigned b = (unsigned)(s % 64);
        if (s > 0 && pl > 0) {
            size_t nl = pl - qs;
            for (i = 0; i < nl; i++) {
                uint64_t lo = P[i + qs];
                uint64_t hi = (i + qs + 1 < pl) ? P[i + qs + 1] : 0;
                P[i] = b ? ((lo >> b) | (hi << (64 - b))) : lo;
            }
            for (i = nl; i < pl; i++) P[i] = 0;
            pl = nl;
            q2297_trim(P, &pl);
        }
    }

    /* remove odd primes p <= n-1 */
    if (N >= 4) {
        size_t M = N - 1;
        bool *comp = (bool *)calloc(M + 1, sizeof(bool));
        size_t p, j;
        for (p = 3; p <= M; p += 2) {
            if (comp[p]) continue;
            for (j = p * p; j <= M; j += 2 * p) comp[j] = true;
            uint64_t e = q2297_legendre((uint64_t)M, (uint64_t)p);
            while (e > 0 && pl > 0 && q2297_mod(P, pl, (uint64_t)p) == 0) {
                q2297_div(P, &pl, (uint64_t)p);
                e--;
            }
        }
        free(comp);
    }

    seq_t res = 0;
    for (i = pl; i-- > 0;) {
        int j;
        for (j = 7; j >= 0; j--) {
            int byte = (int)((P[i] >> (8 * j)) & 255);
            res = res * 256 + byte;
        }
    }
    free(C); free(T); free(P); free(Q);
    return res;
}
