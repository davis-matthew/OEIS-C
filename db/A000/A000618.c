/*
 * A000618  Number of nondegenerate Boolean functions of n variables: For n >
 *          0, a(n) = A000616(n) - A000616(n-1).
 * https://oeis.org/A000618
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(0) = A000616(0) = 2; for n > 0, a(n) = A000616(n) -
 *            A000616(n-1), where A000616(n) = (1/(2^n n!)) * Sum_{g in B_n}
 *            2^{cyc(g)} (Burnside over the hyperoctahedral group acting on
 *            {0,1}^n by permuting/complementing variables); cyc(g) is the
 *            number of cycles of g on {0,1}^n
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 5, u64 n <= 6, u128 n <= 7,
 *            big any n
 * Helpers:   q618_gcd, q618_phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/q618_gcd.h"
#include "../utils/q618_phi.h"

typedef struct { uint32_t *d; size_t len, cap; } q618_bn;

static unsigned q618_N;
static unsigned *q618_pos, *q618_neg;
static q618_bn q618_S, q618_G, q618_C;

static void q618_init(q618_bn *a, size_t cap) {
    a->d = (uint32_t *)calloc(cap, sizeof(uint32_t));
    a->cap = cap; a->len = 0;
}

static void q618_trim(q618_bn *a) {
    while (a->len && a->d[a->len - 1] == 0) a->len--;
}

static void q618_mul(q618_bn *a, uint32_t m) {
    uint64_t c = 0; size_t i;
    for (i = 0; i < a->len; i++) {
        uint64_t t = (uint64_t)a->d[i] * m + c;
        a->d[i] = (uint32_t)t; c = t >> 32;
    }
    if (c) a->d[a->len++] = (uint32_t)c;
}

static void q618_div(q618_bn *a, uint32_t m) {
    uint64_t r = 0; size_t i = a->len;
    while (i-- > 0) {
        uint64_t t = (r << 32) | a->d[i];
        a->d[i] = (uint32_t)(t / m); r = t % m;
    }
    q618_trim(a);
}

static void q618_addsh(q618_bn *s, const q618_bn *c, uint64_t sh) {
    size_t w = (size_t)(sh / 32), i, j;
    unsigned b = (unsigned)(sh % 32);
    uint64_t carry = 0;
    for (i = 0; i <= c->len; i++) {
        uint32_t lo = i < c->len ? c->d[i] : 0;
        uint32_t prev = i > 0 ? c->d[i - 1] : 0;
        uint32_t piece = b ? (uint32_t)((lo << b) | (prev >> (32 - b))) : lo;
        uint64_t t = (uint64_t)s->d[w + i] + piece + carry;
        s->d[w + i] = (uint32_t)t; carry = t >> 32;
    }
    j = w + i;
    while (carry) {
        uint64_t t = (uint64_t)s->d[j] + carry;
        s->d[j] = (uint32_t)t; carry = t >> 32; j++;
    }
    if (j > s->len) s->len = j;
    q618_trim(s);
}

static void q618_proc(void) {
    unsigned N = q618_N, k, t;
    uint64_t L = 1, d;
    unsigned __int128 cyc = 0;
    for (k = 1; k <= N; k++) {
        if (q618_pos[k]) L = L / q618_gcd(L, k) * k;
        if (q618_neg[k]) L = L / q618_gcd(L, 2 * (uint64_t)k) * (2 * (uint64_t)k);
    }
    for (d = 1; d <= L; d++) {
        unsigned e = 0; int zero = 0;
        if (L % d) continue;
        for (k = 1; k <= N && !zero; k++) {
            if (q618_pos[k]) e += q618_pos[k] * (unsigned)q618_gcd(k, d);
            if (q618_neg[k]) {
                uint64_t g = q618_gcd(k, d);
                if ((d / g) % 2) zero = 1;
                else e += q618_neg[k] * (unsigned)g;
            }
        }
        if (!zero) cyc += (unsigned __int128)q618_phi(L / d) << e;
    }
    cyc /= L;
    memcpy(q618_C.d, q618_G.d, q618_G.cap * sizeof(uint32_t));
    q618_C.len = q618_G.len;
    for (k = 1; k <= N; k++) {
        for (t = 0; t < q618_pos[k] + q618_neg[k]; t++) q618_div(&q618_C, 2 * k);
        for (t = 2; t <= q618_pos[k]; t++) q618_div(&q618_C, t);
        for (t = 2; t <= q618_neg[k]; t++) q618_div(&q618_C, t);
    }
    q618_addsh(&q618_S, &q618_C, (uint64_t)cyc);
}

static void q618_rec(unsigned k, unsigned rem) {
    unsigned m, j;
    if (rem == 0) { q618_proc(); return; }
    if (k == 0) return;
    for (m = 0; m * k <= rem; m++)
        for (j = 0; j <= m; j++) {
            q618_pos[k] = j; q618_neg[k] = m - j;
            q618_rec(k - 1, rem - m * k);
        }
    q618_pos[k] = 0; q618_neg[k] = 0;
}

/* returns A000616(n) as a bignum (caller frees d) */
static q618_bn q618_a616(unsigned n) {
    unsigned i, bl = 1;
    size_t gcap, scap;
    q618_bn res;
    while ((1u << bl) <= n) bl++;
    gcap = (size_t)n * (bl + 1) / 32 + 4;
    scap = ((size_t)1 << n) / 32 + gcap + 4;
    q618_N = n;
    q618_pos = (unsigned *)calloc(n + 2, sizeof(unsigned));
    q618_neg = (unsigned *)calloc(n + 2, sizeof(unsigned));
    q618_init(&q618_G, gcap);
    q618_init(&q618_C, gcap);
    q618_init(&q618_S, scap);
    q618_G.d[0] = 1; q618_G.len = 1;
    for (i = 0; i < n; i++) q618_mul(&q618_G, 2);
    for (i = 2; i <= n; i++) q618_mul(&q618_G, i);
    q618_rec(n, n);
    for (i = 0; i < n; i++) q618_div(&q618_S, 2);
    for (i = 2; i <= n; i++) q618_div(&q618_S, i);
    res = q618_S;
    free(q618_G.d); free(q618_C.d);
    free(q618_pos); free(q618_neg);
    return res;
}

seq_t a(idx_t n) {
    q618_bn A = q618_a616((unsigned)n);
    seq_t r = 0;
    size_t i;
    if (n > 0) {
        q618_bn B = q618_a616((unsigned)(n - 1));
        uint64_t borrow = 0;
        for (i = 0; i < A.len; i++) {
            uint64_t bv = i < B.len ? B.d[i] : 0;
            uint64_t av = A.d[i];
            uint64_t t = av - bv - borrow;
            borrow = (av < bv + borrow) ? 1 : 0;
            A.d[i] = (uint32_t)t;
        }
        q618_trim(&A);
        free(B.d);
    }
    i = A.len;
    while (i-- > 0) {
        int s;
        for (s = 24; s >= 0; s -= 8) {
            r = r * 256;
            r = r + (seq_t)((A.d[i] >> s) & 255u);
        }
    }
    free(A.d);
    return r;
}
