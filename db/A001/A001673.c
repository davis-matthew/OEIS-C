/*
 * A001673  a(n) = ceiling(Pi^n).
 * https://oeis.org/A001673
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(Pi^n); Pi from Machin's formula Pi =
 *            16*arctan(1/5) - 4*arctan(1/239) in fixed point, then Pi^n by
 *            binary powering with all roundings downward, so a(n) =
 *            floor(computed Pi^n) + 1 for n >= 1
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   p1673_add, p1673_atan, p1673_divs, p1673_mulfx, p1673_muls,
 *            p1673_sub, p1673_zero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/p1673_add.h"
#include "../utils/p1673_atan.h"
#include "../utils/p1673_divs.h"
#include "../utils/p1673_mulfx.h"
#include "../utils/p1673_muls.h"
#include "../utils/p1673_sub.h"
#include "../utils/p1673_zero.h"

seq_t a(idx_t n) {
    size_t FL, M, i, lx, lb;
    uint32_t *a5, *a239, *X, *B;
    uint64_t c;
    idx_t e;
    seq_t r;
    int s;

    if (n == 0) return 1;

    FL = (size_t)((17 * n) / 10 + 96) / 32 + 1;
    M = FL + 3; /* 2 guard limbs + 1 integer limb */

    a5 = (uint32_t *)malloc(M * sizeof(uint32_t));
    a239 = (uint32_t *)malloc(M * sizeof(uint32_t));
    p1673_atan(a5, M, 5);
    p1673_atan(a239, M, 239);
    p1673_muls(a5, M, 16);
    p1673_muls(a239, M, 4);
    p1673_sub(a5, a239, M);
    /* subtract a margin exceeding the series error so Pi is underestimated */
    memset(a239, 0, M * sizeof(uint32_t));
    a239[0] = (uint32_t)1 << 20;
    p1673_sub(a5, a239, M);
    free(a239);

    lb = FL + 1;
    B = (uint32_t *)malloc(lb * sizeof(uint32_t));
    memcpy(B, a5 + 2, lb * sizeof(uint32_t));
    free(a5);

    lx = FL + 1;
    X = (uint32_t *)calloc(lx, sizeof(uint32_t));
    X[FL] = 1;

    e = n;
    while (e) {
        if (e & 1) {
            size_t nl;
            uint32_t *t = p1673_mulfx(X, lx, B, lb, FL, &nl);
            free(X);
            X = t;
            lx = nl;
        }
        e >>= 1;
        if (e) {
            size_t nl;
            uint32_t *t = p1673_mulfx(B, lb, B, lb, FL, &nl);
            free(B);
            B = t;
            lb = nl;
        }
    }
    free(B);

    r = 0;
    c = 0;
    for (i = lx; i > FL; i--) {
        uint32_t w = X[i - 1];
        if (c == 0 && w == 0) continue;
        c = 1;
        for (s = 24; s >= 0; s -= 8) {
            r <<= 8;
            r |= (seq_t)((w >> s) & 255u);
        }
    }
    free(X);
    return r + 1;
}
