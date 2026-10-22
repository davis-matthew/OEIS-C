/*
 * A003457  a(n) = ceiling(Bernoulli(2n)/(-4n)).
 * https://oeis.org/A003457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = ceiling((-1)^n * T(n) / (2*4^n*(4^n-1))), where T(n) =
 *            A000182(n) are the tangent numbers, since Bernoulli(2n) =
 *            (-1)^(n-1)*2n*T(n)/(4^n*(4^n-1)). T(n) via T(k) = (k-1)*T(k-1),
 *            then T(j) = (j-k)*T(j-1) + (j-k+2)*T(j) for 2<=k<=j
 * Range:     i8 n <= 10, i16 n <= 13, i32 n <= 16, i64 n <= 22, i128 n <= 32,
 *            big any n
 * Helpers:   t3457_add, t3457_allones, t3457_bitlen, t3457_mask, t3457_shr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/t3457_add.h"
#include "../utils/t3457_allones.h"
#include "../utils/t3457_bitlen.h"
#include "../utils/t3457_mask.h"
#include "../utils/t3457_shr.h"

static uint32_t *t3457_T = NULL;
static size_t *t3457_len = NULL;
static idx_t t3457_N = 0;
static size_t t3457_L = 0;

static void t3457_build(idx_t N) {
    size_t lg = 1, L, i;
    idx_t j, k;
    while (((idx_t)1 << lg) < 2 * N) lg++;
    L = (size_t)((2 * N * lg + 64) / 32 + 2);
    free(t3457_T);
    free(t3457_len);
    t3457_T = (uint32_t *)calloc((size_t)(N + 1) * L, sizeof(uint32_t));
    t3457_len = (size_t *)calloc((size_t)(N + 1), sizeof(size_t));
    t3457_T[1 * L] = 1;
    t3457_len[1] = 1;
    for (k = 2; k <= N; k++) {
        const uint32_t *y = t3457_T + (size_t)(k - 1) * L;
        uint32_t *x = t3457_T + (size_t)k * L;
        size_t ln = t3457_len[k - 1];
        uint64_t carry = 0;
        for (i = 0; i < ln; i++) {
            uint64_t v = (uint64_t)y[i] * (uint64_t)(k - 1) + carry;
            x[i] = (uint32_t)v;
            carry = v >> 32;
        }
        if (carry) x[ln++] = (uint32_t)carry;
        t3457_len[k] = ln;
    }
    for (k = 2; k <= N; k++) {
        for (j = k; j <= N; j++) {
            uint64_t c1 = (uint64_t)(j - k), c2 = (uint64_t)(j - k + 2);
            uint32_t *x = t3457_T + (size_t)j * L;
            const uint32_t *y = t3457_T + (size_t)(j - 1) * L;
            size_t ln = t3457_len[j] > t3457_len[j - 1] ? t3457_len[j] : t3457_len[j - 1];
            uint64_t carry = 0;
            for (i = 0; i < ln; i++) {
                uint64_t v = (uint64_t)x[i] * c2 + (uint64_t)y[i] * c1 + carry;
                x[i] = (uint32_t)v;
                carry = v >> 32;
            }
            if (carry) x[ln++] = (uint32_t)carry;
            while (ln > 0 && x[ln - 1] == 0) ln--;
            t3457_len[j] = ln;
        }
    }
    t3457_N = N;
    t3457_L = L;
}

seq_t a(idx_t n) {
    size_t L, ln, W, i;
    const uint32_t *x;
    uint32_t *Y, *h, *q;
    uint64_t m = 2 * (uint64_t)n;
    int rem;
    seq_t res = 0;

    if (n > t3457_N) {
        idx_t N = 2 * t3457_N;
        if (N < n) N = n;
        if (N < 32) N = 32;
        t3457_build(N);
    }
    L = t3457_L;
    ln = t3457_len[n];
    x = t3457_T + (size_t)n * L;
    W = ln + 2;
    Y = (uint32_t *)calloc(W, sizeof(uint32_t));
    h = (uint32_t *)calloc(W, sizeof(uint32_t));
    q = (uint32_t *)calloc(W, sizeof(uint32_t));
    memcpy(h, x, ln * sizeof(uint32_t));

    /* Y = floor(T / 2^(2n+1)) */
    rem = t3457_shr(Y, h, W, m + 1);

    /* q = floor(Y / (2^m - 1)) */
    while (t3457_bitlen(Y, W) > m) {
        t3457_shr(h, Y, W, m);
        t3457_mask(Y, W, m);
        t3457_add(q, h, W);
        t3457_add(Y, h, W);
    }
    if (t3457_allones(Y, W, m)) {
        memset(Y, 0, W * sizeof(uint32_t));
        memset(h, 0, W * sizeof(uint32_t));
        h[0] = 1;
        t3457_add(q, h, W);
    }
    for (i = 0; i < W; i++)
        if (Y[i]) rem = 1;

    i = W;
    while (i > 0 && q[i - 1] == 0) i--;
    while (i > 0) {
        uint32_t w = q[i - 1];
        res = res * 65536 + (int)(w >> 16);
        res = res * 65536 + (int)(w & 0xFFFFu);
        i--;
    }
    free(Y);
    free(h);
    free(q);

    if (n % 2 == 0) {
        if (rem) res += 1;
    } else {
        res = -res;
    }
    return res;
}
