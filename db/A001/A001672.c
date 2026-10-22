/*
 * A001672  a(n) = floor(Pi^n).
 * https://oeis.org/A001672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(Pi^n); Pi via Machin's formula Pi = 16*arctan(1/5)
 *            - 4*arctan(1/239) in multi-limb fixed point, then raised to the
 *            n-th power by repeated squaring with enough guard bits that the
 *            floor is certain (precision is raised if it is not)
 * Range:     u8 n <= 4, u16 n <= 9, u32 n <= 19, u64 n <= 38, u128 n <= 77,
 *            big any n
 * Helpers:   pin_add, pin_atan_inv, pin_divsmall, pin_iszero, pin_mul,
 *            pin_mulsmall, pin_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/pin_add.h"
#include "../utils/pin_atan_inv.h"
#include "../utils/pin_divsmall.h"
#include "../utils/pin_iszero.h"
#include "../utils/pin_mul.h"
#include "../utils/pin_mulsmall.h"
#include "../utils/pin_sub.h"

seq_t a(idx_t n)
{
    uint64_t ib, lgn = 0, m;
    size_t I, g;
    if (n == 0) return (seq_t)1;
    ib = (uint64_t)n * 1652 / 1000 + 3;
    I = (size_t)(ib / 32) + 2;
    for (m = 4 * (uint64_t)n + 4; m; m >>= 1) lgn++;
    for (g = 0;; g++) {
        size_t k = g + 1;
        size_t F = (size_t)((ib + lgn + 8 + 31) / 32) + k + 1;
        size_t L = I + F, M = F + 3, i;
        uint32_t *A = (uint32_t *)calloc(M, sizeof(uint32_t));
        uint32_t *B = (uint32_t *)calloc(M, sizeof(uint32_t));
        uint32_t *P = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *R = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *T = (uint32_t *)calloc(L, sizeof(uint32_t));
        uint32_t *tmp = (uint32_t *)calloc(2 * L, sizeof(uint32_t));
        int top, b, allz = 1, allo = 1;
        seq_t r = 0;

        pin_atan_inv(A, M, 5);
        pin_mulsmall(A, M, 16);
        pin_atan_inv(B, M, 239);
        pin_mulsmall(B, M, 4);
        pin_sub(A, B, M);
        memcpy(P, A + 2, (F + 1) * sizeof(uint32_t));

        memcpy(R, P, L * sizeof(uint32_t));
        top = 63;
        while (!((n >> top) & 1)) top--;
        for (b = top - 1; b >= 0; b--) {
            pin_mul(R, R, T, tmp, L, F);
            memcpy(R, T, L * sizeof(uint32_t));
            if ((n >> b) & 1) {
                pin_mul(R, P, T, tmp, L, F);
                memcpy(R, T, L * sizeof(uint32_t));
            }
        }

        for (i = 0; i < k; i++) {
            uint32_t v = R[F - 1 - i];
            if (v != 0) allz = 0;
            if (v != 0xFFFFFFFFu) allo = 0;
        }
        if (!allz && !allo) {
            size_t li = L;
            while (li > F && R[li - 1] == 0) li--;
            while (li > F) {
                uint32_t v;
                int s;
                li--;
                v = R[li];
                for (s = 24; s >= 0; s -= 8) {
                    r *= 256;
                    r += (unsigned)((v >> s) & 0xFF);
                }
            }
        }
        free(A); free(B); free(P); free(R); free(T); free(tmp);
        if (!allz && !allo) return r;
    }
}
