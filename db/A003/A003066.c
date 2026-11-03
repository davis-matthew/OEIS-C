/*
 * A003066  Problimes (first definition).
 * https://oeis.org/A003066
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   a(n+1) = a(n) + floor( Product_{i=1..n} a(i)/(a(i)-1) ), a(1) =
 *            2 (exact big-rational arithmetic on Q = Product a(i), P =
 *            Product (a(i)-1))
 * Range:     u8 n <= 48, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 74 terms through n = 970 (2026-11-01)
 * Helpers:   bit_length_u32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bit_length_u32.h"

static void add_n(uint32_t *x, size_t *xl, const uint32_t *y, size_t yl) {
    size_t L = *xl > yl ? *xl : yl, i;
    uint64_t c = 0;
    for (i = 0; i < L; i++) {
        uint64_t s = c + (i < *xl ? x[i] : 0) + (i < yl ? y[i] : 0);
        x[i] = (uint32_t)s;
        c = s >> 32;
    }
    if (c) x[L++] = (uint32_t)c;
    *xl = L;
}

static int cmp_n(const uint32_t *x, size_t xl, const uint32_t *y, size_t yl) {
    size_t i;
    if (xl != yl) return xl < yl ? -1 : 1;
    for (i = xl; i-- > 0;) {
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    }
    return 0;
}

static uint64_t get64(const uint32_t *x, size_t len, uint64_t pos) {
    size_t w = (size_t)(pos / 32);
    unsigned b = (unsigned)(pos % 32);
    unsigned __int128 v = 0;
    int k;
    for (k = 2; k >= 0; k--) {
        v <<= 32;
        if (w + k < len) v |= x[w + k];
    }
    return (uint64_t)(v >> b);
}

static void mul_small(uint32_t *x, size_t *xl, uint32_t m) {
    uint64_t c = 0;
    size_t i;
    for (i = 0; i < *xl; i++) {
        uint64_t s = (uint64_t)x[i] * m + c;
        x[i] = (uint32_t)s;
        c = s >> 32;
    }
    while (c) { x[(*xl)++] = (uint32_t)c; c >>= 32; }
    while (*xl > 0 && x[*xl - 1] == 0) (*xl)--;
}

static void sub_n(uint32_t *x, size_t *xl, const uint32_t *y, size_t yl) {
    size_t i;
    int64_t b = 0;
    for (i = 0; i < *xl; i++) {
        int64_t s = (int64_t)x[i] - (i < yl ? (int64_t)y[i] : 0) - b;
        if (s < 0) { s += ((int64_t)1 << 32); b = 1; } else b = 0;
        x[i] = (uint32_t)s;
    }
    while (*xl > 0 && x[*xl - 1] == 0) (*xl)--;
}

static uint32_t *QQ = 0, *PP = 0, *TT = 0, *T2 = 0;
static size_t QL = 0, PL = 0, CAP = 0;
static uint64_t *TERMS = 0;
static size_t TN = 0, TC = 0;

static void ensure_cap(size_t need) {
    if (need <= CAP) return;
    size_t nc = CAP ? CAP : 16;
    while (nc < need) nc *= 2;
    QQ = (uint32_t *)realloc(QQ, nc * sizeof(uint32_t));
    PP = (uint32_t *)realloc(PP, nc * sizeof(uint32_t));
    TT = (uint32_t *)realloc(TT, nc * sizeof(uint32_t));
    T2 = (uint32_t *)realloc(T2, nc * sizeof(uint32_t));
    CAP = nc;
}

/* floor(QQ / PP), quotient known to be small */
static uint64_t small_quot(void) {
    uint64_t bl = 32 * (uint64_t)(PL - 1) + bit_length_u32(PP[PL - 1]);
    uint64_t pos = bl > 64 ? bl - 64 : 0;
    unsigned __int128 X = (unsigned __int128)get64(QQ, QL, pos)
                        | ((unsigned __int128)get64(QQ, QL, pos + 64) << 64);
    uint64_t Y = get64(PP, PL, pos);
    uint64_t q = (uint64_t)(X / Y);
    size_t tl, t2l;
    if (q > 0) q--;
    /* TT = q * P */
    memcpy(TT, PP, PL * sizeof(uint32_t));
    tl = PL;
    if (q == 0) tl = 0; else mul_small(TT, &tl, (uint32_t)q);
    while (cmp_n(TT, tl, QQ, QL) > 0) { sub_n(TT, &tl, PP, PL); q--; }
    for (;;) {
        memcpy(T2, TT, tl * sizeof(uint32_t));
        t2l = tl;
        add_n(T2, &t2l, PP, PL);
        if (cmp_n(T2, t2l, QQ, QL) <= 0) {
            memcpy(TT, T2, t2l * sizeof(uint32_t));
            tl = t2l;
            q++;
        } else break;
    }
    return q;
}

static void extend_to(size_t n) {
    if (TN == 0) {
        TC = 64;
        TERMS = (uint64_t *)malloc(TC * sizeof(uint64_t));
        TERMS[0] = 2;
        TN = 1;
        ensure_cap(16);
        QQ[0] = 2; QL = 1;
        PP[0] = 1; PL = 1;
    }
    while (TN < n) {
        uint64_t nx;
        ensure_cap(QL + 8);
        nx = TERMS[TN - 1] + small_quot();
        if (TN >= TC) {
            TC *= 2;
            TERMS = (uint64_t *)realloc(TERMS, TC * sizeof(uint64_t));
        }
        TERMS[TN++] = nx;
        mul_small(QQ, &QL, (uint32_t)nx);
        mul_small(PP, &PL, (uint32_t)(nx - 1));
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    extend_to((size_t)n);
    return (seq_t)TERMS[n - 1];
}
