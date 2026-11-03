/*
 * A006832  Discriminants of totally real cubic fields.
 * https://oeis.org/A006832
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. Enumerate the GL2(Z)-reduced binary
 *            cubic forms (a,b,c,d) of discriminant 0 < D <= X, where reduced
 *            means the Hessian (P,Q,R) satisfies 0 <= Q <= P <= R. Keep the
 *            forms that are irreducible and maximal at every p
 *            (Davenport-Heilbronn), so each one is O_K of exactly one...
 * Range:     u8 n <= 5, u16 n <= 3049, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9747 (2026-11-01)
 * Helpers:   div_ceil
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/div_ceil.h"

/* f counts only if it is lexicographically maximal in its orbit under Stab(H) */
static bool canonical(long long a, long long b, long long c, long long d,
                      long long P, long long Q, long long R) {
    int al, be, ga, de;
    for (al = -1; al <= 1; al++)
    for (be = -1; be <= 1; be++)
    for (ga = -1; ga <= 1; ga++)
    for (de = -1; de <= 1; de++) {
        long long det = (long long)al * de - (long long)be * ga;
        long long P2, Q2, R2, a2, b2, c2, d2;
        if (det != 1 && det != -1) continue;
        P2 = P * al * al + Q * al * ga + R * ga * ga;
        R2 = P * be * be + Q * be * de + R * de * de;
        Q2 = 2 * P * al * be + Q * (al * de + be * ga) + 2 * R * ga * de;
        if (P2 != P || Q2 != Q || R2 != R) continue;
        a2 = a * al * al * al + b * al * al * ga + c * al * ga * ga + d * ga * ga * ga;
        d2 = a * be * be * be + b * be * be * de + c * be * de * de + d * de * de * de;
        b2 = 3 * a * al * al * be + b * (al * al * de + 2 * al * be * ga)
           + c * (2 * al * ga * de + be * ga * ga) + 3 * d * ga * ga * de;
        c2 = 3 * a * al * be * be + b * (2 * al * be * de + be * be * ga)
           + c * (al * de * de + 2 * be * ga * de) + 3 * d * ga * de * de;
        a2 *= det; b2 *= det; c2 *= det; d2 *= det;
        if (a2 > a) return false;
        if (a2 < a) continue;
        if (b2 > b) return false;
        if (b2 < b) continue;
        if (c2 > c) return false;
        if (c2 < c) continue;
        if (d2 > d) return false;
    }
    return true;
}

static int cmpll(const void *x, const void *y) {
    long long u = *(const long long *)x, v = *(const long long *)y;
    return (u > v) - (u < v);
}

static long long fdivll(long long x, long long y) {
    long long q = x / y;
    if ((x % y != 0) && ((x < 0) != (y < 0))) q--;
    return q;
}

static bool max_at(long long a, long long b, long long c, long long d, long long p) {
    long long p2 = p * p, x;
    if (a % p == 0 && b % p == 0 && c % p == 0 && d % p == 0) return false;
    if (a % p == 0 && b % p == 0 && a % p2 == 0) return false;
    for (x = 0; x < p; x++) {
        long long fx = ((a * x + b) * x + c) * x + d;
        if (fx % p == 0) {
            long long dx = (3 * a * x + 2 * b) * x + c;
            if (dx % p == 0 && fx % p2 == 0) return false;
        }
    }
    return true;
}

static bool maximal(long long a, long long b, long long c, long long d, long long D) {
    long long rem = D, p;
    for (p = 2; p * p <= rem; p++) {
        int e = 0;
        if (rem % p) continue;
        while (rem % p == 0) { rem /= p; e++; }
        if (e >= 2 && !max_at(a, b, c, d, p)) return false;
    }
    return true;
}

static bool root_test(long long a, long long b, long long c, long long d, long long p) {
    long long q, s;
    for (q = 1; q <= a; q++) {
        if (a % q) continue;
        for (s = -1; s <= 1; s += 2) {
            __int128 P = (__int128)(s * p);
            __int128 v = (__int128)a * P * P * P + (__int128)b * P * P * q
                       + (__int128)c * P * q * q + (__int128)d * q * q * q;
            if (v == 0) return true;
        }
    }
    return false;
}

static bool reducible(long long a, long long b, long long c, long long d) {
    long long ad, i;
    if (d == 0) return true;
    ad = d < 0 ? -d : d;
    for (i = 1; i * i <= ad; i++) {
        if (ad % i) continue;
        if (root_test(a, b, c, d, i)) return true;
        if (root_test(a, b, c, d, ad / i)) return true;
    }
    return false;
}

static long long *dl = NULL;
static idx_t dcnt = 0;
static idx_t dcap = 0;

static long long isq(long long x) {
    long long r = 0;
    while ((r + 1) * (r + 1) <= x) r++;
    return r;
}

static void push(long long v) {
    if (dcnt == dcap) {
        dcap = dcap ? dcap * 2 : 1024;
        dl = (long long *)realloc(dl, (size_t)dcap * sizeof(long long));
    }
    dl[dcnt++] = v;
}

static void build(long long X) {
    long long s = isq(X);
    long long t = isq(s / 3 + 1) + 1;
    long long a;
    dcnt = 0;
    for (a = 1; 729 * a * a * a * a <= 16 * X; a++) {
        long long Bb = 2 * a + 6 * t + 2, b;
        for (b = -Bb; b <= Bb; b++) {
            long long clo = div_ceil(b * b - s, 3 * a);
            long long chi = fdivll(b * b - 1, 3 * a);
            long long c;
            for (c = clo; c <= chi; c++) {
                long long P = b * b - 3 * a * c;
                long long dlo, dhi, d;
                if (P <= 0 || P > s) continue;
                dlo = div_ceil(b * c - P, 9 * a);
                dhi = fdivll(b * c, 9 * a);
                for (d = dlo; d <= dhi; d++) {
                    long long Q = b * c - 9 * a * d;
                    long long R = c * c - 3 * b * d;
                    long long D;
                    if (Q < 0 || Q > P || P > R) continue;
                    D = (4 * P * R - Q * Q) / 3;
                    if (D <= 0 || D > X) continue;
                    if (!canonical(a, b, c, d, P, Q, R)) continue;
                    if (reducible(a, b, c, d)) continue;
                    if (!maximal(a, b, c, d, D)) continue;
                    push(D);
                }
            }
        }
    }
    qsort(dl, (size_t)dcnt, sizeof(long long), cmpll);
}

seq_t a(idx_t n) {
    if (dl == NULL || dcnt < n) {
        long long X = 30 * (long long)n + 2000;
        for (;;) {
            build(X);
            if (dcnt >= n) break;
            X *= 2;
        }
    }
    return (seq_t)dl[n - 1];
}
