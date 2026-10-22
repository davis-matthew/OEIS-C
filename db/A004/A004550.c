/*
 * A004550  Expansion of sqrt(3) in base 5.
 * https://oeis.org/A004550
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(3) * 5^(n-1)) mod 5, i.e. a(n) =
 *            isqrt(3*25^(n-1)) mod 5; the digits come from a digit-by-digit
 *            square root in base 5, with bignums in 32-bit limbs
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a4550_cmp, a4550_mulsmall
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a4550_cmp.h"
#include "../utils/a4550_mulsmall.h"

static uint32_t *A4550_R = NULL, *A4550_P = NULL, *A4550_T = NULL;
static size_t A4550_cap = 0, A4550_rl = 0, A4550_pl = 0;
static unsigned char *A4550_dig = NULL;
static idx_t A4550_nd = 0, A4550_dcap = 0;

static void a4550_ensure(void) {
    size_t need = (size_t)(A4550_nd / 8) + 16;
    if (A4550_cap < need) {
        size_t nc = need * 2;
        A4550_R = (uint32_t *)realloc(A4550_R, nc * sizeof(uint32_t));
        A4550_P = (uint32_t *)realloc(A4550_P, nc * sizeof(uint32_t));
        A4550_T = (uint32_t *)realloc(A4550_T, nc * sizeof(uint32_t));
        A4550_cap = nc;
    }
    if (A4550_dcap <= A4550_nd) {
        idx_t nc = A4550_dcap ? A4550_dcap * 2 : 64;
        A4550_dig = (unsigned char *)realloc(A4550_dig, (size_t)nc);
        A4550_dcap = nc;
    }
}

static void a4550_step(void) {
    uint32_t d;
    size_t tl = 0;
    a4550_ensure();
    /* bring down next pair of base-5 digits: "03" first, then "00" */
    a4550_mulsmall(A4550_R, &A4550_rl, 25, A4550_nd == 0 ? 3u : 0u);
    for (d = 4; d >= 1; d--) {
        memcpy(A4550_T, A4550_P, A4550_pl * sizeof(uint32_t));
        tl = A4550_pl;
        a4550_mulsmall(A4550_T, &tl, 10, d);   /* 2*5*p + d */
        a4550_mulsmall(A4550_T, &tl, d, 0);    /* (2*5*p + d) * d */
        if (a4550_cmp(A4550_T, tl, A4550_R, A4550_rl) <= 0) break;
    }
    if (d >= 1) {
        uint64_t borrow = 0;
        size_t i;
        for (i = 0; i < A4550_rl; i++) {
            uint64_t sub = (i < tl ? (uint64_t)A4550_T[i] : 0) + borrow;
            uint64_t cur = A4550_R[i];
            if (cur >= sub) {
                A4550_R[i] = (uint32_t)(cur - sub);
                borrow = 0;
            } else {
                A4550_R[i] = (uint32_t)(cur + ((uint64_t)1 << 32) - sub);
                borrow = 1;
            }
        }
        while (A4550_rl > 0 && A4550_R[A4550_rl - 1] == 0) A4550_rl--;
    } else {
        d = 0;
    }
    a4550_mulsmall(A4550_P, &A4550_pl, 5, d);
    A4550_dig[A4550_nd++] = (unsigned char)d;
}

seq_t a(idx_t n) {
    while (A4550_nd < n) a4550_step();
    return (seq_t)A4550_dig[n - 1];
}
