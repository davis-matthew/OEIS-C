/*
 * A002364  Numerators of continued fraction convergents to fifth root of 5.
 * https://oeis.org/A002364
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, frac
 * Formula:   a(n) = c(n)*a(n-1) + a(n-2), a(-1) = 1, a(0) = c(0) = 1, where
 *            c(k) are the continued fraction terms of 5^(1/5). The c(k) come
 *            from exact integer arithmetic on x^5 - 5: take c = floor(root of
 *            P), then set P(y) := y^5 * P(c + 1/y)
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 21, u64 n <= 29, u128 n <= 29,
 *            big any n
 * Helpers:   bitlen_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/bitlen_u64.h"

static uint64_t *pbuf = 0;
static uint64_t *cp[6];
static uint64_t *ptmp = 0;
static size_t L = 0;

static uint64_t *cfv = 0;
static size_t ncf = 0, capcf = 0;

static size_t free_bits(const uint64_t *x) {
    uint64_t sext = (x[L - 1] >> 63) ? ~(uint64_t)0 : 0;
    size_t i = L - 1;
    while (i > 0 && x[i] == sext && ((x[i - 1] >> 63) == (sext & 1))) i--;
    return 64 * (L - i - 1);
}

static void ensure_room(size_t need) {
    size_t mn = (size_t)-1;
    int k;
    for (k = 0; k < 6; k++) {
        size_t f = free_bits(cp[k]);
        if (f < mn) mn = f;
    }
    if (mn >= need) return;
    {
        size_t newL = L + need / 64 + 4;
        uint64_t *nb = (uint64_t *)malloc(sizeof(uint64_t) * 6 * newL);
        for (k = 0; k < 6; k++) {
            uint64_t *dst = nb + (size_t)k * newL;
            uint64_t sext = (cp[k][L - 1] >> 63) ? ~(uint64_t)0 : 0;
            size_t i;
            memcpy(dst, cp[k], sizeof(uint64_t) * L);
            for (i = L; i < newL; i++) dst[i] = sext;
        }
        free(pbuf);
        pbuf = nb;
        for (k = 0; k < 6; k++) cp[k] = pbuf + (size_t)k * newL;
        free(ptmp);
        ptmp = (uint64_t *)malloc(sizeof(uint64_t) * newL);
        L = newL;
    }
}

static void big_add(uint64_t *d, const uint64_t *s) {
    uint64_t carry = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        uint64_t x = d[i] + carry;
        uint64_t c1 = (x < carry);
        uint64_t y = x + s[i];
        uint64_t c2 = (y < x);
        d[i] = y;
        carry = c1 | c2;
    }
}

static void big_addmul(uint64_t *d, const uint64_t *s, uint64_t m) {
    unsigned __int128 carry = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        unsigned __int128 t = (unsigned __int128)s[i] * m + d[i] + carry;
        d[i] = (uint64_t)t;
        carry = t >> 64;
    }
}

static void big_mulsmall(uint64_t *x, uint64_t m) {
    unsigned __int128 carry = 0;
    size_t i;
    for (i = 0; i < L; i++) {
        unsigned __int128 t = (unsigned __int128)x[i] * m + carry;
        x[i] = (uint64_t)t;
        carry = t >> 64;
    }
}

/* true iff the unique real root of the current polynomial exceeds t */
static int root_gt(uint64_t t) {
    int j;
    ensure_room((size_t)(5 * bitlen_u64(t) + 8));
    memcpy(ptmp, cp[5], sizeof(uint64_t) * L);
    for (j = 4; j >= 0; j--) {
        big_mulsmall(ptmp, t);
        big_add(ptmp, cp[j]);
    }
    return (ptmp[L - 1] >> 63) != (cp[5][L - 1] >> 63);
}

static void init_poly(void) {
    int k;
    size_t i;
    L = 4;
    pbuf = (uint64_t *)malloc(sizeof(uint64_t) * 6 * L);
    ptmp = (uint64_t *)malloc(sizeof(uint64_t) * L);
    for (k = 0; k < 6; k++) {
        cp[k] = pbuf + (size_t)k * L;
        for (i = 0; i < L; i++) cp[k][i] = 0;
    }
    for (i = 0; i < L; i++) cp[0][i] = ~(uint64_t)0;
    cp[0][0] = (uint64_t)0 - 5;   /* -5 */
    cp[5][0] = 1;                 /* x^5 */
}

static void next_term(void) {
    uint64_t c;
    int i, j;
    uint64_t *t;
    if (!root_gt(1)) {
        c = 0;
    } else {
        uint64_t lo = 1, hi = 2;
        while (root_gt(hi)) { lo = hi; hi *= 2; }
        while (hi - lo > 1) {
            uint64_t mid = lo + (hi - lo) / 2;
            if (root_gt(mid)) lo = mid; else hi = mid;
        }
        c = lo;
    }
    if (c > 0) {
        ensure_room((size_t)(5 * bitlen_u64(c + 1) + 8));
        for (i = 0; i < 5; i++)
            for (j = 4; j >= i; j--)
                big_addmul(cp[j], cp[j + 1], c);
    }
    t = cp[0]; cp[0] = cp[5]; cp[5] = t;
    t = cp[1]; cp[1] = cp[4]; cp[4] = t;
    t = cp[2]; cp[2] = cp[3]; cp[3] = t;

    if (ncf == capcf) {
        size_t nc = capcf ? capcf * 2 : 64;
        uint64_t *nv = (uint64_t *)malloc(sizeof(uint64_t) * nc);
        if (ncf) memcpy(nv, cfv, sizeof(uint64_t) * ncf);
        free(cfv);
        cfv = nv;
        capcf = nc;
    }
    cfv[ncf++] = c;
}

seq_t a(idx_t n) {
    seq_t p2, p1;
    idx_t i;
    if (pbuf == 0) init_poly();
    while ((idx_t)ncf <= n) next_term();
    p2 = 1;
    p1 = (seq_t)cfv[0];
    for (i = 1; i <= n; i++) {
        seq_t ai = (seq_t)cfv[i];
        seq_t p = ai * p1 + p2;
        p2 = p1;
        p1 = p;
    }
    return p1;
}
