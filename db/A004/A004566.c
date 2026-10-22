/*
 * A004566  Expansion of sqrt(6) in base 7.
 * https://oeis.org/A004566
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(6) * 7^(n-1)) mod 7, i.e.
 *            floor(sqrt(6*49^(n-1))) mod 7, computed by the digit-by-digit
 *            square root in base 7 on arrays of base-7 digits
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmp_digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/cmp_digits.h"

static int *P = 0, *R = 0, *T = 0, *V = 0;
static size_t plen = 0, rlen = 0, cap = 0;
static unsigned char *D = 0;
static size_t nd = 0, dcap = 0;

static void ensure(size_t need) {
    if (cap < need) {
        size_t nc = need * 2 + 16;
        P = (int *)realloc(P, nc * sizeof(int));
        R = (int *)realloc(R, nc * sizeof(int));
        T = (int *)realloc(T, nc * sizeof(int));
        V = (int *)realloc(V, nc * sizeof(int));
        cap = nc;
    }
}

static void step(void) {
    size_t i, tlen, vlen = 0, L, k;
    int c, d;
    long long rt = 0, tt = 0;

    ensure((plen > rlen ? plen : rlen) + 8);

    /* r = r * 49 (bring down the next pair "00") */
    if (rlen) {
        for (i = rlen; i-- > 0;) R[i + 2] = R[i];
        R[0] = 0;
        R[1] = 0;
        rlen += 2;
    }

    /* t = 2 * 7 * p */
    T[0] = 0;
    c = 0;
    for (i = 0; i < plen; i++) {
        int x = 2 * P[i] + c;
        T[i + 1] = x % 7;
        c = x / 7;
    }
    tlen = plen + 1;
    if (c) T[tlen++] = c;

    /* estimate the next digit from leading digits */
    L = rlen > tlen ? rlen : tlen;
    k = L > 8 ? L - 8 : 0;
    for (i = L; i-- > k;) {
        rt = rt * 7 + (i < rlen ? R[i] : 0);
        tt = tt * 7 + (i < tlen ? T[i] : 0);
    }
    d = tt ? (int)(rt / tt) + 1 : 6;
    if (d > 6) d = 6;

    for (; d > 0; d--) {
        c = 0;
        for (i = 0; i < tlen; i++) {
            int x = (i == 0 ? d : T[i]) * d + c;
            V[i] = x % 7;
            c = x / 7;
        }
        vlen = tlen;
        while (c) {
            V[vlen++] = c % 7;
            c /= 7;
        }
        while (vlen > 0 && V[vlen - 1] == 0) vlen--;
        if (cmp_digits(V, vlen, R, rlen) <= 0) break;
    }

    if (d > 0) {
        int br = 0;
        for (i = 0; i < rlen; i++) {
            int x = R[i] - (i < vlen ? V[i] : 0) - br;
            if (x < 0) {
                x += 7;
                br = 1;
            } else {
                br = 0;
            }
            R[i] = x;
        }
        while (rlen > 0 && R[rlen - 1] == 0) rlen--;
    }

    /* p = p * 7 + d */
    for (i = plen; i-- > 0;) P[i + 1] = P[i];
    P[0] = d;
    plen++;

    if (nd >= dcap) {
        dcap = dcap * 2 + 64;
        D = (unsigned char *)realloc(D, dcap);
    }
    D[nd++] = (unsigned char)d;
}

seq_t a(idx_t n) {
    if (nd == 0) {
        ensure(16);
        dcap = 64;
        D = (unsigned char *)malloc(dcap);
        P[0] = 2;
        plen = 1;
        R[0] = 2; /* 6 - 2*2 */
        rlen = 1;
        D[0] = 2;
        nd = 1;
    }
    while (nd < n) step();
    return (seq_t)D[n - 1];
}
