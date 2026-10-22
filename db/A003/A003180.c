/*
 * A003180  Number of equivalence classes of Boolean functions of n variables
 *          under action of symmetric group.
 * https://oeis.org/A003180
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n) = Sum_{1*s_1+2*s_2+...=n} 2^c(s) /
 *            (1^s_1*s_1!*2^s_2*s_2!*...), where c(s) = (1/L) * Sum_{k=0..L-1}
 *            2^(Sum_j s_j*gcd(j,k)), L = lcm of the cycle lengths (c(s) is
 *            the number of cycles a permutation of cycle type s induces on
 *            the subsets of an n-set). Exact arithmetic uses an internal
 *            multi-li...
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   a3180_div, a3180_gcd, a3180_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a3180_div.h"
#include "../utils/a3180_gcd.h"
#include "../utils/a3180_mul.h"

static uint32_t *a3180_acc;
static size_t a3180_cap;
static int a3180_n;

static void a3180_term(const int *parts, const int *mults, int np) {
    int n = a3180_n;
    uint64_t L = 1;
    int i;
    for (i = 0; i < np; i++) L = L / a3180_gcd(L, (uint64_t)parts[i]) * (uint64_t)parts[i];
    uint64_t sum = 0, k;
    for (k = 0; k < L; k++) {
        uint64_t e = 0;
        for (i = 0; i < np; i++) e += (uint64_t)mults[i] * a3180_gcd((uint64_t)parts[i], k);
        sum += (uint64_t)1 << e;
    }
    uint64_t c = sum / L;

    size_t ccap = (size_t)n * 8 / 32 + 4;
    uint32_t *coef = (uint32_t *)calloc(ccap, sizeof(uint32_t));
    size_t clen = 1;
    coef[0] = 1;
    for (i = 2; i <= n; i++) a3180_mul(coef, &clen, (uint32_t)i);
    for (i = 0; i < np; i++) {
        int t;
        for (t = 1; t <= mults[i]; t++) {
            a3180_div(coef, &clen, (uint32_t)parts[i]);
            a3180_div(coef, &clen, (uint32_t)t);
        }
    }

    size_t word = (size_t)(c / 32);
    unsigned bit = (unsigned)(c % 32);
    uint64_t carry = 0, spill = 0;
    size_t j;
    for (j = 0; j <= clen || carry; j++) {
        uint64_t sh = (j < clen) ? ((uint64_t)coef[j] << bit) : 0;
        uint64_t part = (sh & 0xffffffffULL) + spill;
        spill = sh >> 32;
        uint64_t v = (uint64_t)a3180_acc[word + j] + part + carry;
        a3180_acc[word + j] = (uint32_t)v;
        carry = v >> 32;
    }
    free(coef);
}

static void a3180_rec(int rem, int maxp, int *parts, int *mults, int np) {
    if (rem == 0) { a3180_term(parts, mults, np); return; }
    int p;
    for (p = (maxp < rem ? maxp : rem); p >= 1; p--) {
        int m;
        for (m = 1; m * p <= rem; m++) {
            parts[np] = p;
            mults[np] = m;
            a3180_rec(rem - m * p, p - 1, parts, mults, np + 1);
        }
    }
}

seq_t a(idx_t n) {
    int nn = (int)n;
    a3180_n = nn;
    a3180_cap = (((size_t)1 << nn) + (size_t)8 * nn + 64) / 32 + 8;
    a3180_acc = (uint32_t *)calloc(a3180_cap, sizeof(uint32_t));
    int *parts = (int *)malloc(sizeof(int) * (nn + 2));
    int *mults = (int *)malloc(sizeof(int) * (nn + 2));

    a3180_rec(nn, nn, parts, mults, 0);

    size_t len = a3180_cap;
    int i;
    for (i = 2; i <= nn; i++) a3180_div(a3180_acc, &len, (uint32_t)i);

    seq_t r = 0;
    size_t j = len;
    while (j > 0) {
        j--;
        int b;
        for (b = 3; b >= 0; b--) {
            r = (seq_t)(r << 8);
            r = r + (seq_t)((a3180_acc[j] >> (8 * b)) & 0xffu);
        }
    }
    free(parts);
    free(mults);
    free(a3180_acc);
    a3180_acc = NULL;
    return r;
}
