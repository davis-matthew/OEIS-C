/*
 * A005988  x^3 + n*y^3 = 1 is solvable.
 * https://oeis.org/A005988
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   search, not a closed form. a(n) is the n-th number m >= 2 with m
 *            = (x^3 - 1)/y^3 or m = (x^3 + 1)/y^3 for integers x >= 1, y >=
 *            1, i.e. x^3 + m*y^3 = 1 has a solution with y != 0. For each y,
 *            the x with x^3 == 1 (mod y^3) are found by lifting roots mod y
 *            -> y^2 -> y^3; then all m <= K^3 are liste...
 * Range:     u8 n <= 19, u16 n <= 135, u32 n <= 135, u64 n <= 135,
 *            u128 n <= 135, big any n
 * Helpers:   cube_is_one
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <stdlib.h>
#include "../utils/cube_is_one.h"

#define YMAX 2500

static uint64_t *res_s = NULL;   /* residues s mod y^3 with s^3 == 1 */
static uint32_t *res_y = NULL;
static size_t nres = 0, capres = 0;

static uint64_t *terms = NULL;
static idx_t nterms = 0;
static uint64_t curK = 0;

static void push_res(uint32_t y, uint64_t s) {
    if (nres == capres) {
        capres = capres ? capres * 2 : 1024;
        res_s = (uint64_t *)realloc(res_s, capres * sizeof(uint64_t));
        res_y = (uint32_t *)realloc(res_y, capres * sizeof(uint32_t));
    }
    res_s[nres] = s;
    res_y[nres] = y;
    nres++;
}

static void init_residues(void) {
    uint64_t *l1 = (uint64_t *)malloc(sizeof(uint64_t) * (YMAX + 1));
    size_t cap2 = 64, cap3 = 64;
    uint64_t *l2 = (uint64_t *)malloc(sizeof(uint64_t) * cap2);
    uint64_t *l3 = (uint64_t *)malloc(sizeof(uint64_t) * cap3);
    for (uint64_t y = 1; y <= YMAX; y++) {
        uint64_t y2 = y * y, y3 = y2 * y;
        size_t n1 = 0, n2 = 0, n3 = 0;
        for (uint64_t r = 0; r < y; r++)
            if (cube_is_one(r, y)) l1[n1++] = r;
        for (size_t i = 0; i < n1; i++)
            for (uint64_t t = 0; t < y; t++) {
                uint64_t x = l1[i] + t * y;
                if (cube_is_one(x, y2)) {
                    if (n2 == cap2) { cap2 *= 2; l2 = (uint64_t *)realloc(l2, sizeof(uint64_t) * cap2); }
                    l2[n2++] = x;
                }
            }
        for (size_t i = 0; i < n2; i++)
            for (uint64_t t = 0; t < y; t++) {
                uint64_t x = l2[i] + t * y2;
                if (cube_is_one(x, y3)) {
                    if (n3 == cap3) { cap3 *= 2; l3 = (uint64_t *)realloc(l3, sizeof(uint64_t) * cap3); }
                    l3[n3++] = x;
                }
            }
        for (size_t i = 0; i < n3; i++) push_res((uint32_t)y, l3[i]);
    }
    free(l1); free(l2); free(l3);
}

static int cmp_u64(const void *p, const void *q) {
    uint64_t a1 = *(const uint64_t *)p, b1 = *(const uint64_t *)q;
    return (a1 > b1) - (a1 < b1);
}

static void build(uint64_t K) {
    uint64_t N = K * K * K;
    size_t cap = 1024, cnt = 0;
    uint64_t *v = (uint64_t *)malloc(cap * sizeof(uint64_t));
    for (size_t i = 0; i < nres; i++) {
        uint64_t y = res_y[i], y3 = y * y * y;
        uint64_t s = res_s[i];
        uint64_t xmax = y * K + 1;
        for (int e = 0; e < 2; e++) {
            /* e == 0: x^3 == 1 (mod y^3), m = (x^3-1)/y^3
               e == 1: x^3 == -1 (mod y^3), m = (x^3+1)/y^3 */
            uint64_t start = (e == 0) ? s : (s == 0 ? 0 : y3 - s);
            for (uint64_t x = start; x <= xmax; x += y3) {
                unsigned __int128 x3 = (unsigned __int128)x * x * x;
                unsigned __int128 num;
                if (e == 0) {
                    if (x3 < 1) continue;
                    num = x3 - 1;
                } else {
                    num = x3 + 1;
                }
                unsigned __int128 m = num / y3;
                if (m < 2 || m > N) continue;
                if (cnt == cap) { cap *= 2; v = (uint64_t *)realloc(v, cap * sizeof(uint64_t)); }
                v[cnt++] = (uint64_t)m;
            }
        }
    }
    qsort(v, cnt, sizeof(uint64_t), cmp_u64);
    size_t u = 0;
    for (size_t i = 0; i < cnt; i++)
        if (u == 0 || v[u - 1] != v[i]) v[u++] = v[i];
    free(terms);
    terms = v;
    nterms = (idx_t)u;
    curK = K;
}

seq_t a(idx_t n) {
    if (res_s == NULL) init_residues();
    if (curK == 0) build(16);
    while (nterms < n) build(curK * 2);
    return (seq_t)terms[n - 1];
}
