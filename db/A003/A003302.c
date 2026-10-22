/*
 * A003302  Denominators of coefficients of Green function for cubic lattice.
 * https://oeis.org/A003302
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = denominator of r(n), where 9(n+1)(2n+1)(2n+3) r(n+1) =
 *            2(2n+1)(10n^2+10n+3) r(n) - 4n^3 r(n-1), r(0) = 1. Write r(n) =
 *            N(n)/D(n) with D(n) = 9^n * n! * (2n+1)!!. Then N(n+1) =
 *            2(10n^2+10n+3) N(n) - 36 n^4 N(n-1), N(0) = 1, N(-1) = 0, and
 *            a(n) = D(n) / gcd(N(n), D(n)). The gcd is found prim...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 8, u64 n <= 14, u128 n <= 15,
 *            big any n
 * Helpers:   a3302_addmul, a3302_div, a3302_mod, a3302_mul, a3302_norm,
 *            a3302_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3302_addmul.h"
#include "../utils/a3302_div.h"
#include "../utils/a3302_mod.h"
#include "../utils/a3302_mul.h"
#include "../utils/a3302_norm.h"
#include "../utils/a3302_sub.h"

static uint32_t *a3302_Pa = NULL, *a3302_Pb = NULL, *a3302_Pt = NULL;
static size_t a3302_La = 0, a3302_Lb = 0, a3302_cap = 0;
static idx_t a3302_K = 0;
static int a3302_ready = 0;

static void a3302_ensure(size_t need)
{
    if (need <= a3302_cap) return;
    size_t nc = need * 2;
    a3302_Pa = (uint32_t *)realloc(a3302_Pa, nc * sizeof(uint32_t));
    a3302_Pb = (uint32_t *)realloc(a3302_Pb, nc * sizeof(uint32_t));
    a3302_Pt = (uint32_t *)realloc(a3302_Pt, nc * sizeof(uint32_t));
    a3302_cap = nc;
}

seq_t a(idx_t n)
{
    if (!a3302_ready || n < a3302_K) {
        a3302_ensure(16);
        a3302_Pa[0] = 1; a3302_La = 1;
        a3302_Pb[0] = 0; a3302_Lb = 1;
        a3302_K = 0;
        a3302_ready = 1;
    }
    while (a3302_K < n) {
        idx_t K = a3302_K;
        size_t mx = a3302_La > a3302_Lb ? a3302_La : a3302_Lb;
        size_t Lt;
        uint32_t *tmp;
        a3302_ensure(mx + 8);
        memcpy(a3302_Pt, a3302_Pa, a3302_La * sizeof(uint32_t));
        Lt = a3302_La;
        Lt = a3302_mul(a3302_Pt, Lt, 10);
        Lt = a3302_mul(a3302_Pt, Lt, (uint32_t)K);
        Lt = a3302_mul(a3302_Pt, Lt, (uint32_t)(K + 1));
        Lt = a3302_addmul(a3302_Pt, Lt, a3302_Pa, a3302_La, 3);
        Lt = a3302_mul(a3302_Pt, Lt, 2);
        a3302_Lb = a3302_mul(a3302_Pb, a3302_Lb, 36);
        a3302_Lb = a3302_mul(a3302_Pb, a3302_Lb, (uint32_t)K);
        a3302_Lb = a3302_mul(a3302_Pb, a3302_Lb, (uint32_t)K);
        a3302_Lb = a3302_mul(a3302_Pb, a3302_Lb, (uint32_t)K);
        a3302_Lb = a3302_mul(a3302_Pb, a3302_Lb, (uint32_t)K);
        Lt = a3302_sub(a3302_Pt, Lt, a3302_Pb, a3302_Lb);
        tmp = a3302_Pb;
        a3302_Pb = a3302_Pa; a3302_Lb = a3302_La;
        a3302_Pa = a3302_Pt; a3302_La = Lt;
        a3302_Pt = tmp;
        a3302_K = K + 1;
    }

    size_t L = a3302_La;
    uint32_t *W = (uint32_t *)malloc((L + 1) * sizeof(uint32_t));
    memcpy(W, a3302_Pa, L * sizeof(uint32_t));
    uint64_t M = 2 * (uint64_t)n + 1;
    unsigned char *comp = (unsigned char *)calloc((size_t)M + 1, 1);
    seq_t res = 1;
    uint64_t p;
    for (p = 2; p <= M; p++) {
        uint64_t e = 0, f = 0, t, q, i;
        if (comp[p]) continue;
        for (q = p * p; q <= M; q += p) comp[q] = 1;
        if (p == 2) {
            t = n;
            while (t) { t /= 2; e += t; }
        } else {
            t = M;
            while (t) { t /= p; e += t; }
            if (p == 3) e += 2 * (uint64_t)n;
        }
        while (f < e) {
            uint64_t pk = 1, j = 0, r;
            while (j < e - f && pk <= 0xFFFFFFFFull / p) { pk *= p; j++; }
            r = a3302_mod(W, L, pk);
            if (r == 0) {
                L = a3302_div(W, L, pk);
                f += j;
            } else {
                while (r % p == 0) { r /= p; f++; }
                break;
            }
        }
        for (i = f; i < e; i++) res *= (seq_t)p;
    }
    free(comp);
    free(W);
    return res;
}
