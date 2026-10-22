/*
 * A004552  Expansion of sqrt(3) in base 7.
 * https://oeis.org/A004552
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(3) * 7^(n-1)) mod 7, computed by
 *            digit-by-digit base-7 square root of 3 with internal
 *            multi-precision integers
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   mul_small_add, sub_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mul_small_add.h"
#include "../utils/sub_big.h"

static uint32_t *P = NULL, *R = NULL, *T = NULL, *Y = NULL;
static size_t lp = 0, lr = 0, lt = 0, ly = 0, cap = 0;
static unsigned char *dig = NULL;
static size_t nd = 0, dcap = 0;

static int cmp_big(const uint32_t *a, size_t la, const uint32_t *b, size_t lb) {
    size_t i;
    if (la != lb) return la < lb ? -1 : 1;
    for (i = la; i-- > 0;) {
        if (a[i] != b[i]) return a[i] < b[i] ? -1 : 1;
    }
    return 0;
}

static void ensure_cap(size_t k) {
    size_t need = k / 8 + 16;
    if (need > cap) {
        size_t nc = need * 2;
        P = (uint32_t *)realloc(P, nc * sizeof(uint32_t));
        R = (uint32_t *)realloc(R, nc * sizeof(uint32_t));
        T = (uint32_t *)realloc(T, nc * sizeof(uint32_t));
        Y = (uint32_t *)realloc(Y, nc * sizeof(uint32_t));
        cap = nc;
    }
    if (k > dcap) {
        size_t nc = k * 2 + 64;
        dig = (unsigned char *)realloc(dig, nc);
        dcap = nc;
    }
}

static void step(void) {
    uint32_t pair = (nd == 0) ? 3u : 0u;
    uint32_t d;
    lr = mul_small_add(R, lr, 49u, pair);
    memcpy(T, P, lp * sizeof(uint32_t));
    lt = mul_small_add(T, lp, 14u, 0u);
    for (d = 6;; d--) {
        memcpy(Y, T, lt * sizeof(uint32_t));
        ly = mul_small_add(Y, lt, d, d * d);
        if (cmp_big(Y, ly, R, lr) <= 0) break;
        if (d == 0) break;
    }
    lr = sub_big(R, lr, Y, ly);
    lp = mul_small_add(P, lp, 7u, d);
    dig[nd++] = (unsigned char)d;
}

seq_t a(idx_t n) {
    size_t target = (size_t)n;
    if (target > nd) {
        ensure_cap(target + 1);
        while (nd < target) step();
    }
    return (seq_t)dig[n - 1];
}
