/*
 * A004588  Expansion of sqrt(10) in base 5.
 * https://oeis.org/A004588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(10) * 5^(n-1)) mod 5, computed by the
 *            digit-by-digit base-5 square root algorithm with custom
 *            multi-precision limbs
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   cmpbig, growz, subbig
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/cmpbig.h"
#include "../utils/growz.h"
#include "../utils/subbig.h"

static uint32_t *P = NULL, *C = NULL, *T = NULL;
static size_t pl = 0, cl = 0, cap = 0;
static unsigned char *dig = NULL;
static idx_t cnt = 0, dcap = 0;

static void mulsmall(uint32_t *x, size_t *len, uint32_t m, uint32_t add) {
    uint64_t carry = add;
    size_t i;
    for (i = 0; i < *len; i++) {
        uint64_t v = (uint64_t)x[i] * m + carry;
        x[i] = (uint32_t)v;
        carry = v >> 32;
    }
    if (carry) x[(*len)++] = (uint32_t)carry;
}

static void extend(idx_t N) {
    size_t need = (size_t)(N * 5 / 32 + 16);
    if (need > cap) {
        P = growz(P, cap, need);
        C = growz(C, cap, need);
        T = growz(T, cap, need);
        cap = need;
    }
    if (N > dcap) {
        dig = (unsigned char *)realloc(dig, (size_t)N);
        dcap = N;
    }
    while (cnt < N) {
        uint32_t d = (cnt == 0) ? 10u : 0u;
        uint32_t x;
        mulsmall(C, &cl, 25u, 0u);
        if (cl == 0 && d) { C[0] = d; cl = 1; }
        else if (d) {
            /* add d to C (C*25 already applied) */
            uint64_t carry = d; size_t i = 0;
            while (carry && i < cl) { uint64_t v = (uint64_t)C[i] + carry; C[i] = (uint32_t)v; carry = v >> 32; i++; }
            if (carry) C[cl++] = (uint32_t)carry;
        }
        for (x = 4; x >= 1; x--) {
            size_t tl = pl;
            memcpy(T, P, pl * sizeof(uint32_t));
            mulsmall(T, &tl, 10u, x);
            if (tl == 0) { T[0] = x; tl = 1; }
            mulsmall(T, &tl, x, 0u);
            if (cmpbig(T, tl, C, cl) <= 0) {
                subbig(C, &cl, T, tl);
                break;
            }
        }
        mulsmall(P, &pl, 5u, x);
        if (pl == 0 && x) { P[0] = x; pl = 1; }
        dig[cnt++] = (unsigned char)x;
    }
}

seq_t a(idx_t n) {
    if (n < 1) return (seq_t)0;
    if (n > cnt) extend(n + 64);
    return (seq_t)dig[n - 1];
}
