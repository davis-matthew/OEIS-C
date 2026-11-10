/*
 * A205782  Least positive integer k such that n divides C(k)-C(j) for some j
 *          in [1,k-1], where C=A205824.
 * https://oeis.org/A205782
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = least k with C(k) == C(j) (mod n) for some j<k,
 *            C(k)=A205824(k)=(3k-1)!/(k!(k+1)!); found by tracking C(k) mod n
 *            via prime exponents
 * Range:     u8 n <= 93, u16 n <= 93, u32 n <= 93, u64 n <= 93, u128 n <= 93,
 *            big any n
 * Validated: partial, pass, 93 terms through n = 93 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

typedef unsigned __int128 A205782_u128;

static uint64_t A205782_mm(uint64_t a, uint64_t b, uint64_t m) {
    return (uint64_t)(((A205782_u128)a * b) % m);
}

static uint64_t A205782_inv(uint64_t a, uint64_t m) {
    __int128 r0 = (__int128)m, r1 = (__int128)(a % m);
    __int128 t0 = 0, t1 = 1;
    while (r1 != 0) {
        __int128 q = r0 / r1;
        __int128 r2 = r0 - q * r1; r0 = r1; r1 = r2;
        __int128 t2 = t0 - q * t1; t0 = t1; t1 = t2;
    }
    if (t0 < 0) t0 += (__int128)m;
    return (uint64_t)t0;
}

static void A205782_absorb(uint64_t x, int sign, const uint64_t *ps, int *e, int np,
                           uint64_t *u, uint64_t n) {
    for (int i = 0; i < np; i++) {
        while (x % ps[i] == 0) { x /= ps[i]; e[i] += sign; }
    }
    uint64_t r = x % n;
    if (sign > 0) *u = A205782_mm(*u, r, n);
    else *u = A205782_mm(*u, A205782_inv(r, n), n);
}

static uint64_t A205782_val(uint64_t u, const uint64_t *ps, const int *e, int np, uint64_t n) {
    uint64_t v = u;
    for (int i = 0; i < np; i++) {
        for (int j = 0; j < e[i]; j++) v = A205782_mm(v, ps[i] % n, n);
    }
    return v;
}

seq_t a(idx_t n) {
    if (n <= 1) return (seq_t)2;
    uint64_t N = (uint64_t)n;
    uint64_t ps[24];
    int e[24];
    int np = 0;
    uint64_t t = N;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            ps[np] = p; e[np] = 0; np++;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) { ps[np] = t; e[np] = 0; np++; }

    unsigned char *seen = (unsigned char *)calloc((size_t)N, 1);
    if (!seen) abort();
    uint64_t u = 1 % N;
    seen[(size_t)(1 % N)] = 1;
    for (uint64_t k = 2; k <= N + 1; k++) {
        A205782_absorb(3 * k - 1, 1, ps, e, np, &u, N);
        A205782_absorb(3 * k - 2, 1, ps, e, np, &u, N);
        A205782_absorb(3 * k - 3, 1, ps, e, np, &u, N);
        A205782_absorb(k, -1, ps, e, np, &u, N);
        A205782_absorb(k + 1, -1, ps, e, np, &u, N);
        uint64_t v = A205782_val(u, ps, e, np, N);
        if (seen[(size_t)v]) { free(seen); return (seq_t)k; }
        seen[(size_t)v] = 1;
    }
    free(seen);
    return (seq_t)(N + 1);
}
