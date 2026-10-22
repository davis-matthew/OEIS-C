/*
 * A000518  Generalized tangent numbers d_(n,4).
 * https://oeis.org/A000518
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n,4) = 7! (2n/Pi)^8 L(8, (4n/.)) / sqrt(n) (Shanks),
 *            evaluated exactly via the primitive character chi=(f/.) of
 *            conductor f = disc(Q(sqrt n)), n = s^2*D0: a(n) = -(4n/f)^8
 *            (sqrt(f)/sqrt(n)) / (48 f) * Sum_{k=1..f-1}
 *            chi(k)*3*f^8*B_8(k/f) * Prod_{p|2s, p!|f} (1 - chi(p)/p^8); for
 *            n = s^2...
 * Range:     u8 none, u16 n <= 2, u32 n <= 9, u64 n <= 192, u128 n <= 1000,
 *            big any n
 * Helpers:   k518_jacobi, k518_kron
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <string.h>
#include "../utils/k518_jacobi.h"
#include "../utils/k518_kron.h"

#define K518_NL 16

static void k518_add128(uint32_t *x, unsigned __int128 v)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < K518_NL; i++) {
        c += (uint64_t)x[i] + (uint64_t)(uint32_t)v;
        v >>= 32;
        x[i] = (uint32_t)c;
        c >>= 32;
        if (v == 0 && c == 0) break;
    }
}

static void k518_add(uint32_t *x, const uint32_t *y)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < K518_NL; i++) {
        c += (uint64_t)x[i] + y[i];
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void k518_sub(uint32_t *x, const uint32_t *y)
{
    uint64_t b = 0;
    int i;
    for (i = 0; i < K518_NL; i++) {
        uint64_t t = (uint64_t)x[i] - y[i] - b;
        x[i] = (uint32_t)t;
        b = (t >> 32) ? 1 : 0;
    }
}

static void k518_mul(uint32_t *x, uint32_t m)
{
    uint64_t c = 0;
    int i;
    for (i = 0; i < K518_NL; i++) {
        c += (uint64_t)x[i] * m;
        x[i] = (uint32_t)c;
        c >>= 32;
    }
}

static void k518_div(uint32_t *x, uint32_t d)
{
    uint64_t r = 0;
    int i;
    for (i = K518_NL - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
}

seq_t a(idx_t n)
{
    uint64_t A = (uint64_t)n, m = A, s = 1, D0 = 1, p, q;
    uint64_t ps[64];
    int np = 0, i, j;
    uint32_t N[K518_NL], T[K518_NL];
    seq_t r = 0;

    for (p = 2; p * p <= m; p++) {
        int e = 0;
        while (m % p == 0) { m /= p; e++; }
        for (i = 0; i < e / 2; i++) s *= p;
        if (e & 1) D0 *= p;
    }
    D0 *= m;

    q = 2 * s;
    for (p = 2; p * p <= q; p++) {
        if (q % p == 0) {
            ps[np++] = p;
            while (q % p == 0) q /= p;
        }
    }
    if (q > 1) ps[np++] = q;

    memset(N, 0, sizeof(N));
    if (D0 == 1) {
        N[0] = 8;
        for (i = 0; i < 8; i++) k518_mul(N, (uint32_t)(2 * A));
        for (j = 0; j < np; j++) {
            memcpy(T, N, sizeof(N));
            for (i = 0; i < 8; i++) k518_mul(T, (uint32_t)ps[j]);
            k518_sub(T, N);
            memcpy(N, T, sizeof(N));
        }
        k518_div(N, 15);
        k518_div(N, (uint32_t)s);
        for (j = 0; j < np; j++)
            for (i = 0; i < 8; i++) k518_div(N, (uint32_t)ps[j]);
        if (A == 1) k518_mul(N, 2);
    } else {
        uint64_t f = (D0 % 4 == 1) ? D0 : 4 * D0, k, e;
        uint32_t pos[K518_NL];
        unsigned __int128 F = f, f2 = F * F, f4 = f2 * f2, f6 = f4 * f2;
        memset(pos, 0, sizeof(pos));
        for (k = 1; k < f; k++) {
            int c = k518_kron(f, k);
            unsigned __int128 K, k2, k4, k6, k7, k8, Q;
            if (c == 0) continue;
            K = k; k2 = K * K; k4 = k2 * k2; k6 = k4 * k2; k7 = k6 * K; k8 = k4 * k4;
            Q = 3 * k8 + 14 * k6 * f2 + 2 * k2 * f6 - 12 * k7 * F - 7 * k4 * f4;
            if (c > 0) k518_add128(pos, Q);
            else k518_add128(N, Q);
        }
        k518_sub(N, pos);
        e = 4 * A / f;
        for (i = 0; i < 8; i++) k518_mul(N, (uint32_t)e);
        if (f != D0) k518_mul(N, 2);
        for (j = 0; j < np; j++) {
            int c;
            if (f % ps[j] == 0) continue;
            c = k518_kron(f, ps[j]);
            memcpy(T, N, sizeof(N));
            for (i = 0; i < 8; i++) k518_mul(T, (uint32_t)ps[j]);
            if (c > 0) k518_sub(T, N);
            else k518_add(T, N);
            memcpy(N, T, sizeof(N));
        }
        k518_div(N, 48);
        k518_div(N, (uint32_t)f);
        k518_div(N, (uint32_t)s);
        for (j = 0; j < np; j++) {
            if (f % ps[j] == 0) continue;
            for (i = 0; i < 8; i++) k518_div(N, (uint32_t)ps[j]);
        }
    }

    for (i = K518_NL - 1; i >= 0; i--) {
        for (j = 3; j >= 0; j--) {
            r = r * 256 + (int)((N[i] >> (8 * j)) & 255u);
        }
    }
    return r;
}
