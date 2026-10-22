/*
 * A004581  Expansion of sqrt(8) in base 6.
 * https://oeis.org/A004581
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons
 * Formula:   a(n) = floor(sqrt(8) * 6^(n-1)) mod 6, i.e. a(n) =
 *            floor(sqrt(8*36^(n-1))) mod 6; computed by the digit-by-digit
 *            (schoolbook) square-root algorithm in base 6 with private
 *            multi-limb integers, memoized across calls
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   A004581_cmp, A004581_mulsm, A004581_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/A004581_cmp.h"
#include "../utils/A004581_mulsm.h"
#include "../utils/A004581_sub.h"

static unsigned char *A004581_dig = 0;
static size_t A004581_ndig = 0, A004581_digcap = 0;
static uint32_t *A004581_R = 0, *A004581_P = 0, *A004581_Q = 0, *A004581_V = 0;
static size_t A004581_rl = 0, A004581_pl = 0, A004581_lcap = 0;

static void A004581_ensure(size_t k) {
    size_t need;
    if (k <= A004581_digcap) return;
    {
        size_t nc = A004581_digcap * 2;
        if (nc < k) nc = k;
        if (nc < 64) nc = 64;
        A004581_dig = (unsigned char *)realloc(A004581_dig, nc);
        A004581_digcap = nc;
    }
    need = A004581_digcap / 12 + 8;
    if (need > A004581_lcap) {
        A004581_R = (uint32_t *)realloc(A004581_R, need * sizeof(uint32_t));
        A004581_P = (uint32_t *)realloc(A004581_P, need * sizeof(uint32_t));
        A004581_Q = (uint32_t *)realloc(A004581_Q, need * sizeof(uint32_t));
        A004581_V = (uint32_t *)realloc(A004581_V, need * sizeof(uint32_t));
        A004581_lcap = need;
    }
}

static void A004581_step(void) {
    /* bring down next pair of base-6 digits: 8 = "12"_6 first, then "00" */
    uint32_t pair = (A004581_ndig == 0) ? 8u : 0u;
    size_t ql, vl = 0;
    uint32_t x;
    A004581_rl = A004581_mulsm(A004581_R, A004581_R, A004581_rl, 36, pair);
    ql = A004581_mulsm(A004581_Q, A004581_P, A004581_pl, 12, 0);
    for (x = 5; x > 0; x--) {
        vl = A004581_mulsm(A004581_V, A004581_Q, ql, x, x * x);
        if (A004581_cmp(A004581_V, vl, A004581_R, A004581_rl) <= 0) break;
    }
    if (x > 0)
        A004581_rl = A004581_sub(A004581_R, A004581_rl, A004581_V, vl);
    A004581_pl = A004581_mulsm(A004581_P, A004581_P, A004581_pl, 6, x);
    A004581_dig[A004581_ndig++] = (unsigned char)x;
}

seq_t a(idx_t n) {
    size_t k = (size_t)n;
    if (n < 1) return (seq_t)0;
    A004581_ensure(k);
    while (A004581_ndig < k) A004581_step();
    return (seq_t)A004581_dig[k - 1];
}
