/*
 * A001698  a(n) = ceiling(sqrt( 2*Pi )^n).
 * https://oeis.org/A001698
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceiling(sqrt(2*Pi)^n) = ceiling((2*Pi)^floor(n/2) *
 *            sqrt(2*Pi)^(n mod 2)), computed in exact fixed-point integer
 *            arithmetic: Pi = 16*atan(1/5) - 4*atan(1/239) (Machin),
 *            sqrt(2*Pi) by bitwise integer square root, power by repeated
 *            squaring
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   a1698_add, a1698_addbit, a1698_atan, a1698_cmp, a1698_divs,
 *            a1698_fmul, a1698_isqrt, a1698_iszero, a1698_shr1, a1698_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a1698_add.h"
#include "../utils/a1698_addbit.h"
#include "../utils/a1698_atan.h"
#include "../utils/a1698_cmp.h"
#include "../utils/a1698_divs.h"
#include "../utils/a1698_fmul.h"
#include "../utils/a1698_isqrt.h"
#include "../utils/a1698_iszero.h"
#include "../utils/a1698_shr1.h"
#include "../utils/a1698_sub.h"

seq_t a(idx_t n) {
    static uint32_t *cpi2 = NULL, *cs = NULL;
    static size_t cfl = 0;
    size_t B, fl, off, la, i;
    const uint32_t *pi2, *s;
    uint32_t *acc;
    idx_t k, bit;
    seq_t r;

    if (n == 0) return 1;
    B = (size_t)(n * 1326 / 1000) + 2;
    fl = (B + 160) / 32 + 1;

    if (fl > cfl) {
        size_t nfl = fl, L, NL;
        uint32_t *sa, *sb, *N, *root;
        if (cfl + cfl / 2 > nfl) nfl = cfl + cfl / 2;
        L = nfl + 1;
        sa = (uint32_t *)calloc(L, sizeof(uint32_t));
        sb = (uint32_t *)calloc(L, sizeof(uint32_t));
        a1698_atan(sa, L, nfl, 5, 16);
        a1698_atan(sb, L, nfl, 239, 4);
        a1698_sub(sa, sb, L);
        a1698_add(sa, sa, L); /* 2*Pi */
        free(sb);
        NL = 2 * nfl + 1;
        N = (uint32_t *)calloc(NL, sizeof(uint32_t));
        memcpy(N + nfl, sa, L * sizeof(uint32_t));
        root = a1698_isqrt(N, NL);
        free(N);
        free(cpi2);
        free(cs);
        cs = (uint32_t *)malloc(L * sizeof(uint32_t));
        memcpy(cs, root, L * sizeof(uint32_t));
        free(root);
        cpi2 = sa;
        cfl = nfl;
    }
    off = cfl - fl;
    pi2 = cpi2 + off;
    s = cs + off;

    acc = (uint32_t *)calloc(fl + 1, sizeof(uint32_t));
    acc[fl] = 1;
    la = fl + 1;
    k = n / 2;
    if (k > 0) {
        bit = 1;
        while (bit <= k / 2) bit <<= 1;
        for (; bit > 0; bit >>= 1) {
            size_t nl;
            uint32_t *t = a1698_fmul(acc, la, acc, la, fl, &nl);
            free(acc);
            acc = t;
            la = nl;
            if (k & bit) {
                t = a1698_fmul(acc, la, pi2, fl + 1, fl, &nl);
                free(acc);
                acc = t;
                la = nl;
            }
        }
    }
    if (n & 1) {
        size_t nl;
        uint32_t *t = a1698_fmul(acc, la, s, fl + 1, fl, &nl);
        free(acc);
        acc = t;
        la = nl;
    }

    r = 0;
    for (i = la; i-- > fl;) {
        int sh;
        for (sh = 24; sh >= 0; sh -= 8) {
            r <<= 8;
            r += (acc[i] >> sh) & 255u;
        }
    }
    free(acc);
    r += 1;
    return r;
}
