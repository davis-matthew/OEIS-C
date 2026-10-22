/*
 * A001674  a(n) = floor(sqrt( 2*Pi )^n).
 * https://oeis.org/A001674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((2*Pi)^(n/2)), computed exactly with multi-limb
 *            fixed-point arithmetic: Pi = 16*atan(1/5) - 4*atan(1/239)
 *            (Machin), sqrt(2*Pi) by digit-by-digit integer square root,
 *            (2*Pi)^floor(n/2) by binary powering, times sqrt(2*Pi) when n is
 *            odd
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 24, u64 n <= 48, u128 n <= 96,
 *            big any n
 * Helpers:   a1674_add, a1674_atan_inv, a1674_cmp, a1674_divs, a1674_iszero,
 *            a1674_mulfix, a1674_muls, a1674_shl, a1674_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a1674_add.h"
#include "../utils/a1674_atan_inv.h"
#include "../utils/a1674_cmp.h"
#include "../utils/a1674_divs.h"
#include "../utils/a1674_iszero.h"
#include "../utils/a1674_mulfix.h"
#include "../utils/a1674_muls.h"
#include "../utils/a1674_shl.h"
#include "../utils/a1674_sub.h"

seq_t a(idx_t n) {
    uint64_t bitsInt = (uint64_t)n * 1326 / 1000 + 8;
    size_t I = (size_t)(bitsInt / 32 + 2);
    size_t F = (size_t)(bitsInt / 32 + 5);
    size_t L = I + F;
    size_t R = F + 3, NL = 2 * F + 1;
    uint32_t *c = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *b2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(2 * L, sizeof(uint32_t));
    uint32_t *s = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *res = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *base = (uint32_t *)calloc(L, sizeof(uint32_t));
    uint32_t *tmp = (uint32_t *)calloc(L, sizeof(uint32_t));
    idx_t k = n / 2, e;
    seq_t r = 0;
    size_t i;

    /* c = 2*Pi = 32*atan(1/5) - 8*atan(1/239) */
    a1674_atan_inv(c, 5, L, F, t, s);
    a1674_atan_inv(b2, 239, L, F, t, s);
    a1674_muls(c, L, 32);
    a1674_muls(b2, L, 8);
    a1674_sub(c, b2, L);

    if (n & 1) {
        /* sqrt(2*Pi) in fixed point: isqrt(c * 2^P) */
        uint32_t *N = (uint32_t *)calloc(NL, sizeof(uint32_t));
        uint32_t *rem = (uint32_t *)calloc(R, sizeof(uint32_t));
        uint32_t *root = (uint32_t *)calloc(R, sizeof(uint32_t));
        uint32_t *trial = (uint32_t *)calloc(R, sizeof(uint32_t));
        size_t li;
        for (i = 0; i <= F; i++) N[i + F] = c[i];
        li = NL;
        while (li > 0) {
            int sh;
            li--;
            for (sh = 30; sh >= 0; sh -= 2) {
                uint32_t pair = (N[li] >> sh) & 3u;
                a1674_shl(rem, R, 2);
                rem[0] |= pair;
                memcpy(trial, root, R * sizeof(uint32_t));
                a1674_shl(trial, R, 2);
                trial[0] |= 1u;
                a1674_shl(root, R, 1);
                if (a1674_cmp(rem, trial, R) >= 0) {
                    a1674_sub(rem, trial, R);
                    root[0] |= 1u;
                }
            }
        }
        memset(s, 0, L * sizeof(uint32_t));
        for (i = 0; i < R && i < L; i++) s[i] = root[i];
        free(N); free(rem); free(root); free(trial);
    }

    /* res = c^k */
    memset(res, 0, L * sizeof(uint32_t));
    res[F] = 1;
    memcpy(base, c, L * sizeof(uint32_t));
    e = k;
    while (e) {
        if (e & 1) {
            a1674_mulfix(tmp, res, base, L, F, t);
            memcpy(res, tmp, L * sizeof(uint32_t));
        }
        e >>= 1;
        if (e) {
            a1674_mulfix(tmp, base, base, L, F, t);
            memcpy(base, tmp, L * sizeof(uint32_t));
        }
    }
    if (n & 1) {
        a1674_mulfix(tmp, res, s, L, F, t);
        memcpy(res, tmp, L * sizeof(uint32_t));
    }

    i = L;
    while (i > F) {
        i--;
        r = r * (seq_t)65536;
        r = r * (seq_t)65536;
        r = r + (seq_t)res[i];
    }

    free(c); free(b2); free(t); free(s); free(res); free(base); free(tmp);
    return r;
}
