/*
 * A002117  Apéry's number or Apéry's constant zeta(3). Decimal expansion of
 *          zeta(3) = Sum_{m >= 1} 1/m^3.
 * https://oeis.org/A002117
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cons, nonn, nice, changed
 * Formula:   zeta(3) = (1/64)*Sum_{k>=0}
 *            (-1)^k*(k!)^10*(205*k^2+250*k+77)/((2*k+1)!)^5
 *            (Amdeberhan-Zeilberger); evaluated in binary fixed point, then
 *            a(1) = integer part, a(n) = (n-1)-th decimal digit after the
 *            point
 * Range:     u8 n <= 9872, u16 n <= 9872, u32 n <= 9872, u64 n <= 9872,
 *            u128 n <= 9872, big any n
 * Helpers:   z3_addmul, z3_divsm, z3_mulsm
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/z3_addmul.h"
#include "../utils/z3_divsm.h"
#include "../utils/z3_mulsm.h"

static unsigned char *z3_dig = NULL;
static size_t z3_ndig = 0;

static void z3_compute(size_t D) {
    size_t L = (size_t)((double)D * 3.32192809489 / 32.0) + 4;
    uint32_t *t = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *pos = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *neg = (uint32_t *)calloc(L, sizeof(uint32_t));
    size_t hi = L, i;
    uint64_t k;
    t[L - 1] = 1;
    z3_addmul(pos, L, t, hi, 77);
    for (k = 1; hi > 0; k++) {
        uint32_t k2 = (uint32_t)(k * k);
        uint32_t den = (uint32_t)((2 * k) * (2 * k + 1));
        uint64_t c;
        int j;
        for (j = 0; j < 5; j++) {
            size_t h = hi + 1 < L ? hi + 1 : L;
            z3_mulsm(t, h, k2);
            hi = h;
            while (hi > 0 && t[hi - 1] == 0) hi--;
            z3_divsm(t, hi, den);
            while (hi > 0 && t[hi - 1] == 0) hi--;
        }
        if (hi == 0) break;
        c = 205 * k * k + 250 * k + 77;
        while (c > 0) {
            uint32_t part = c > 0xFFFFFFFFu ? 0xFFFFFFFFu : (uint32_t)c;
            z3_addmul((k & 1) ? neg : pos, L, t, hi, part);
            c -= part;
        }
    }
    {
        int64_t b = 0;
        for (i = 0; i < L; i++) {
            int64_t v = (int64_t)pos[i] - (int64_t)neg[i] + b;
            if (v < 0) { v += ((int64_t)1 << 32); b = -1; } else b = 0;
            pos[i] = (uint32_t)v;
        }
    }
    z3_divsm(pos, L, 64);
    free(z3_dig);
    z3_dig = (unsigned char *)malloc(D + 10);
    z3_dig[0] = (unsigned char)(pos[L - 1] % 10);
    pos[L - 1] = 0;
    {
        size_t got = 1;
        while (got < D + 1) {
            uint32_t chunk;
            char buf[9];
            int j;
            z3_mulsm(pos, L, 1000000000u);
            chunk = pos[L - 1];
            pos[L - 1] = 0;
            for (j = 8; j >= 0; j--) { buf[j] = (char)(chunk % 10); chunk /= 10; }
            for (j = 0; j < 9 && got < D + 1; j++) z3_dig[got++] = (unsigned char)buf[j];
        }
    }
    z3_ndig = D - 20;
    free(t);
    free(pos);
    free(neg);
}

seq_t a(idx_t n) {
    if (n < 1) return 0;
    if (z3_dig == NULL || (size_t)n > z3_ndig) {
        size_t D = (size_t)n + (size_t)n / 4 + 40;
        z3_compute(D);
    }
    return (seq_t)z3_dig[n - 1];
}
