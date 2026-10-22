/*
 * A004584  Expansion of sqrt(8) in base 9.
 * https://oeis.org/A004584
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, cons, easy
 * Formula:   a(n) = floor(sqrt(8) * 9^(n-1)) mod 9 = floor(sqrt(8*81^(n-1)))
 *            mod 9; the integer square root is found with an exact bit-by-bit
 *            square root on 32-bit limbs, then converted to base 9 (all
 *            digits up to N are cached)
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a004584_cmp, a004584_norm, a004584_shl, a004584_sub
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a004584_cmp.h"
#include "../utils/a004584_norm.h"
#include "../utils/a004584_shl.h"
#include "../utils/a004584_sub.h"

static unsigned char *a004584_dig = NULL;
static idx_t a004584_ndig = 0;

static void a004584_compute(idx_t N) {
    size_t cap = (size_t)((7 * N) / 32 + 16);
    uint32_t *m = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *r = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *q = (uint32_t *)calloc(cap, sizeof(uint32_t));
    uint32_t *t = (uint32_t *)calloc(cap, sizeof(uint32_t));
    unsigned char *low = (unsigned char *)calloc((size_t)N + 16, 1);
    size_t ml = 1, rl = 0, ql = 0, tl, B, P, i, cnt = 0;
    idx_t k;
    uint32_t top;

    m[0] = 8;
    for (k = 1; k < N; k++) {
        uint64_t carry = 0;
        size_t j;
        for (j = 0; j < ml; j++) {
            uint64_t v = (uint64_t)m[j] * 81 + carry;
            m[j] = (uint32_t)v;
            carry = v >> 32;
        }
        if (carry) m[ml++] = (uint32_t)carry;
    }

    B = (ml - 1) * 32;
    top = m[ml - 1];
    while (top) { B++; top >>= 1; }
    P = (B + 1) / 2;

    for (i = P; i-- > 0;) {
        uint32_t bits = (m[(2 * i) / 32] >> ((2 * i) % 32)) & 3;
        a004584_shl(r, &rl, 2);
        if (rl == 0) { r[0] = 0; rl = 1; }
        r[0] |= bits;
        a004584_norm(r, &rl);

        memcpy(t, q, ql * sizeof(uint32_t));
        tl = ql;
        a004584_shl(t, &tl, 2);
        if (tl == 0) { t[0] = 0; tl = 1; }
        t[0] |= 1;

        a004584_shl(q, &ql, 1);
        if (a004584_cmp(r, rl, t, tl) >= 0) {
            a004584_sub(r, &rl, t, tl);
            if (ql == 0) { q[0] = 0; ql = 1; }
            q[0] |= 1;
        }
    }

    while (ql > 0) {
        uint64_t rem = 0;
        size_t j;
        int d;
        for (j = ql; j-- > 0;) {
            uint64_t v = (rem << 32) | q[j];
            q[j] = (uint32_t)(v / 387420489u);
            rem = v % 387420489u;
        }
        a004584_norm(q, &ql);
        for (d = 0; d < 9; d++) {
            if (cnt < (size_t)N + 16) low[cnt] = (unsigned char)(rem % 9);
            cnt++;
            rem /= 9;
        }
    }

    free(a004584_dig);
    a004584_dig = (unsigned char *)calloc((size_t)N + 1, 1);
    for (k = 1; k <= N; k++) a004584_dig[k] = low[N - k];
    a004584_ndig = N;

    free(m); free(r); free(q); free(t); free(low);
}

seq_t a(idx_t n) {
    if (n > a004584_ndig) {
        idx_t N = n + n / 4 + 32;
        a004584_compute(N);
    }
    return (seq_t)a004584_dig[n];
}
