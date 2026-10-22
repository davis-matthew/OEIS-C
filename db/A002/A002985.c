/*
 * A002985  Number of trees in an n-node wheel.
 * https://oeis.org/A002985
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A003293(n-1) - A008804(n-3) (the second term only for n
 *            >= 3), where A003293 = Euler transform of ceil(k/2): prod_{k>=1}
 *            1/(1-x^k)^ceil(k/2), computed via m*b(m) = Sum_{k=1..m}
 *            d(k)*b(m-k), b(0) = 1, d(k) = Sum_{j|k} j*ceil(j/2); A008804(m)
 *            = [x^m] 1/((1-x)^2*(1-x^2)*(1-x^4))
 * Range:     u8 n <= 12, u16 n <= 26, u32 n <= 63, u64 n <= 163,
 *            u128 n <= 200, big any n
 * Helpers:   w2985_a008804, w2985_from_limbs
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/w2985_a008804.h"
#include "../utils/w2985_from_limbs.h"

static uint32_t **w2985_b = NULL;  /* exact b(m), little-endian 32-bit limbs */
static size_t *w2985_len = NULL;
static uint64_t *w2985_d = NULL;
static idx_t w2985_cnt = 0, w2985_cap = 0;

static void w2985_need(idx_t N) {
    idx_t m, k, j;
    if (N < w2985_cnt) return;
    if (N >= w2985_cap) {
        idx_t cap = w2985_cap ? w2985_cap : 64;
        while (cap <= N) cap *= 2;
        w2985_b = (uint32_t **)realloc(w2985_b, (size_t)(cap + 1) * sizeof(uint32_t *));
        w2985_len = (size_t *)realloc(w2985_len, (size_t)(cap + 1) * sizeof(size_t));
        free(w2985_d);
        w2985_d = (uint64_t *)calloc((size_t)(cap + 1), sizeof(uint64_t));
        for (j = 1; j <= cap; j++) {
            uint64_t w = (uint64_t)j * ((j + 1) / 2);
            for (k = j; k <= cap; k += j) w2985_d[k] += w;
        }
        w2985_cap = cap;
    }
    if (w2985_cnt == 0) {
        w2985_b[0] = (uint32_t *)malloc(sizeof(uint32_t));
        w2985_b[0][0] = 1;
        w2985_len[0] = 1;
        w2985_cnt = 1;
    }
    for (m = w2985_cnt; m <= N; m++) {
        size_t L = w2985_len[m - 1] + 4, i;
        uint64_t *acc = (uint64_t *)calloc(L + 1, sizeof(uint64_t));
        uint32_t *q = (uint32_t *)malloc(L * sizeof(uint32_t));
        uint64_t carry = 0, rem = 0;
        for (k = 1; k <= m; k++) {
            uint64_t dk = w2985_d[k];
            const uint32_t *x = w2985_b[m - k];
            size_t lx = w2985_len[m - k];
            for (i = 0; i < lx; i++) {
                uint64_t p = dk * (uint64_t)x[i];
                acc[i] += (uint32_t)p;
                acc[i + 1] += p >> 32;
            }
        }
        for (i = 0; i < L; i++) {
            uint64_t v = acc[i] + carry;
            acc[i] = v & 0xffffffffu;
            carry = v >> 32;
        }
        for (i = L; i-- > 0;) {
            uint64_t cur = (rem << 32) | acc[i];
            q[i] = (uint32_t)(cur / (uint64_t)m);
            rem = cur % (uint64_t)m;
        }
        while (L > 1 && q[L - 1] == 0) L--;
        free(acc);
        w2985_b[m] = q;
        w2985_len[m] = L;
    }
    w2985_cnt = N + 1;
}

seq_t a(idx_t n) {
    seq_t res;
    if (n < 1) return 0;
    w2985_need(n - 1);
    res = w2985_from_limbs(w2985_b[n - 1], w2985_len[n - 1]);
    if (n >= 3) res -= w2985_a008804(n - 3);
    return res;
}
