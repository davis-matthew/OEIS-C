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
 * Helpers:   canonical, cdivll, cmpll, fdivll, max_at, maximal, reducible,
 *            root_test
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/canonical.h"
#include "../utils/cdivll.h"
#include "../utils/cmpll.h"
#include "../utils/fdivll.h"
#include "../utils/max_at.h"
#include "../utils/maximal.h"
#include "../utils/reducible.h"
#include "../utils/root_test.h"

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
            long long clo = cdivll(b * b - s, 3 * a);
            long long chi = fdivll(b * b - 1, 3 * a);
            long long c;
            for (c = clo; c <= chi; c++) {
                long long P = b * b - 3 * a * c;
                long long dlo, dhi, d;
                if (P <= 0 || P > s) continue;
                dlo = cdivll(b * c - P, 9 * a);
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
