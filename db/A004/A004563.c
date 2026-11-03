/*
 * A004563  Expansion of sqrt(6) in base 4.
 * https://oeis.org/A004563
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(6) * 4^(n-1)) mod 4, i.e. a(n) =
 *            floor(sqrt(6*16^(n-1))) mod 4; computed by the exact bit-by-bit
 *            integer square root of 6*4^k, using a private multiprecision
 *            state and memoizing every digit produced so far
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 115 terms through n = 9923 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>

static int mp_cmp(const uint32_t *x, size_t xl, const uint32_t *y, size_t yl) {
    size_t i;
    if (xl != yl) return xl < yl ? -1 : 1;
    for (i = xl; i-- > 0;) {
        if (x[i] != y[i]) return x[i] < y[i] ? -1 : 1;
    }
    return 0;
}

static void mp_shl(uint32_t *x, size_t *len, int s, uint32_t add) {
    uint32_t c = add;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint32_t w = x[i];
        x[i] = (w << s) | c;
        c = w >> (32 - s);
    }
    if (c) x[(*len)++] = c;
}

static void mp_sub(uint32_t *x, size_t *xl, const uint32_t *y, size_t yl) {
    uint64_t borrow = 0;
    size_t i;
    for (i = 0; i < *xl; i++) {
        uint64_t yi = (i < yl) ? y[i] : 0;
        uint64_t d = (uint64_t)x[i] - yi - borrow;
        x[i] = (uint32_t)d;
        borrow = (d >> 63) & 1;
    }
    while (*xl > 0 && x[*xl - 1] == 0) (*xl)--;
}

static uint32_t *Ybuf = 0, *Rbuf = 0, *Tbuf = 0;
static size_t ylen = 0, rlen = 0, wcap = 0;
static unsigned char *digs = 0;
static idx_t ndigs = 0;     /* digits a(1..ndigs) known */
static idx_t steps = 0;     /* radicand bit-pairs processed */

static void ensure(idx_t ndig) {
    size_t need = (size_t)(2 * ndig) / 32 + 4;
    if (need > wcap) {
        size_t nc = need * 2;
        Ybuf = (uint32_t *)realloc(Ybuf, nc * sizeof(uint32_t));
        Rbuf = (uint32_t *)realloc(Rbuf, nc * sizeof(uint32_t));
        Tbuf = (uint32_t *)realloc(Tbuf, nc * sizeof(uint32_t));
        wcap = nc;
    }
    digs = (unsigned char *)realloc(digs, (size_t)(ndig + 1));
}

seq_t a(idx_t n) {
    if (n > ndigs) {
        idx_t target = n + 64;
        ensure(target);
        while (ndigs < target) {
            /* radicand 6 = base-4 "12": bit pairs 01, 10, then zeros */
            uint32_t p = (steps == 0) ? 1u : (steps == 1 ? 2u : 0u);
            size_t tlen;
            mp_shl(Rbuf, &rlen, 2, p);
            memcpy(Tbuf, Ybuf, ylen * sizeof(uint32_t));
            tlen = ylen;
            mp_shl(Tbuf, &tlen, 2, 1);
            if (mp_cmp(Rbuf, rlen, Tbuf, tlen) >= 0) {
                mp_sub(Rbuf, &rlen, Tbuf, tlen);
                mp_shl(Ybuf, &ylen, 1, 1);
            } else {
                mp_shl(Ybuf, &ylen, 1, 0);
            }
            steps++;
            if ((steps & 1) == 0) {
                ndigs++;
                digs[ndigs] = (unsigned char)((ylen ? Ybuf[0] : 0) & 3u);
            }
        }
    }
    return (seq_t)digs[n];
}
