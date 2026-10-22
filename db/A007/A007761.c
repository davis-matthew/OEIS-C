/*
 * A007761  (n+1) * a(n+1) - 2 (68*n^2+68*n+27) * a(n) + 6 * n * (772*n^2+35)
 *          * a(n-1) - 2 * (2*n-1)^2 * (68*n^2-68*n+27) * a(n-2) + (2*n-1)^2 *
 *          (n-1) * (2*n-3)^2 * a(n-3) = 0.
 * https://oeis.org/A007761
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   (n+1)*a(n+1) = 2*(68*n^2+68*n+27)*a(n) - 6*n*(772*n^2+35)*a(n-1)
 *            + 2*(2*n-1)^2*(68*n^2-68*n+27)*a(n-2) -
 *            (2*n-1)^2*(n-1)*(2*n-3)^2*a(n-3), a(0) = 1, a(1) = 54, a(2) =
 *            6381, a(3) = 1176900
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   seq_is_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <string.h>
#include "../utils/seq_is_big.h"

/* small non-negative multi-limb integer, used for fixed-width builds so that
   intermediates never overflow */
typedef struct { uint32_t *d; size_t n, cap; } bn;

static void bn_reserve(bn *x, size_t c) {
    if (c > x->cap) {
        size_t nc = c * 2 + 4;
        x->d = (uint32_t *)realloc(x->d, nc * sizeof(uint32_t));
        x->cap = nc;
    }
}

static void bn_set(bn *x, uint64_t v) {
    bn_reserve(x, 2);
    x->n = 0;
    while (v) { x->d[x->n++] = (uint32_t)v; v >>= 32; }
}

static void bn_trim(bn *x) {
    while (x->n && x->d[x->n - 1] == 0) x->n--;
}

/* dst = src * m  (dst may equal src) */
static void bn_mul(bn *dst, bn *src, uint64_t m) {
    size_t sn = src->n, i;
    unsigned __int128 c = 0;
    bn_reserve(dst, sn + 3);
    for (i = 0; i < sn; i++) {
        c += (unsigned __int128)src->d[i] * m;
        dst->d[i] = (uint32_t)c;
        c >>= 32;
    }
    while (c) { dst->d[i++] = (uint32_t)c; c >>= 32; }
    dst->n = i;
    bn_trim(dst);
}

/* dst += src */
static void bn_add(bn *dst, const bn *src) {
    size_t m = dst->n > src->n ? dst->n : src->n, i;
    uint64_t c = 0;
    bn_reserve(dst, m + 1);
    for (i = dst->n; i < m; i++) dst->d[i] = 0;
    for (i = 0; i < m; i++) {
        c += (uint64_t)dst->d[i] + (i < src->n ? src->d[i] : 0);
        dst->d[i] = (uint32_t)c;
        c >>= 32;
    }
    dst->n = m;
    if (c) dst->d[dst->n++] = (uint32_t)c;
}

/* dst -= src, requires dst >= src */
static void bn_sub(bn *dst, const bn *src) {
    size_t i;
    int64_t b = 0;
    for (i = 0; i < dst->n; i++) {
        int64_t t = (int64_t)dst->d[i] - (i < src->n ? (int64_t)src->d[i] : 0) - b;
        if (t < 0) { t += ((int64_t)1 << 32); b = 1; } else b = 0;
        dst->d[i] = (uint32_t)t;
    }
    bn_trim(dst);
}

/* x /= d, d < 2^32, exact */
static void bn_div(bn *x, uint64_t d) {
    uint64_t r = 0;
    size_t i = x->n;
    while (i-- > 0) {
        uint64_t cur = (r << 32) | x->d[i];
        x->d[i] = (uint32_t)(cur / d);
        r = cur % d;
    }
    bn_trim(x);
}

static void bn_copy(bn *dst, const bn *src) {
    bn_reserve(dst, src->n + 1);
    if (src->n) memcpy(dst->d, src->d, src->n * sizeof(uint32_t));
    dst->n = src->n;
}

static seq_t bn_to_seq(const bn *x) {
    seq_t r = 0;
    size_t i = x->n;
    while (i-- > 0) {
        r = r * 65536;
        r = r * 65536;
        r += (seq_t)x->d[i];
    }
    return r;
}

static bn A[4];
static bn P, N, T;
static idx_t acur = 0;
static int ainit = 0;

static seq_t a_fixed(idx_t n) {
    if (!ainit || n < acur) {
        bn_set(&A[0], 1);
        bn_set(&A[1], 54);
        bn_set(&A[2], 6381);
        bn_set(&A[3], 1176900);
        acur = 3;
        ainit = 1;
    }
    while (acur < n) {
        uint64_t k = (uint64_t)acur;
        uint64_t t1 = (2 * k - 1) * (2 * k - 1);
        uint64_t t3 = (2 * k - 3) * (2 * k - 3);
        bn tmp;
        bn_mul(&P, &A[3], 2 * (68 * k * k + 68 * k + 27));
        bn_mul(&T, &A[1], t1);
        bn_mul(&T, &T, 2 * (68 * k * k - 68 * k + 27));
        bn_add(&P, &T);
        bn_mul(&N, &A[2], 6 * k * (772 * k * k + 35));
        bn_mul(&T, &A[0], t1);
        bn_mul(&T, &T, (k - 1) * t3);
        bn_add(&N, &T);
        bn_sub(&P, &N);
        bn_div(&P, k + 1);
        /* rotate: A[0] <- A[1] <- A[2] <- A[3] <- P, reuse old A[0] buffer for P */
        tmp = A[0];
        A[0] = A[1];
        A[1] = A[2];
        A[2] = A[3];
        A[3] = P;
        P = tmp;
        acur++;
    }
    return bn_to_seq(&A[3]);
}

static seq_t B[4];
static idx_t bcur = 0;
static int binit = 0;

static seq_t a_big(idx_t n) {
    if (!binit || n < bcur) {
        B[0] = 1;
        B[1] = 54;
        B[2] = 6381;
        B[3] = 1176900;
        bcur = 3;
        binit = 1;
    }
    while (bcur < n) {
        seq_t k = (seq_t)bcur;
        seq_t t1 = 2 * k - 1;
        seq_t t3 = 2 * k - 3;
        seq_t p, q, v;
        t1 = t1 * t1;
        t3 = t3 * t3;
        p = 2 * (68 * k * k + 68 * k + 27) * B[3]
          + 2 * t1 * (68 * k * k - 68 * k + 27) * B[1];
        q = 6 * k * (772 * k * k + 35) * B[2]
          + t1 * (k - 1) * t3 * B[0];
        v = (p - q) / (k + 1);
        B[0] = B[1];
        B[1] = B[2];
        B[2] = B[3];
        B[3] = v;
        bcur++;
    }
    return B[3];
}

seq_t a(idx_t n) {
    if (n == 0) return 1;
    if (n == 1) return 54;
    if (n == 2) return 6381;
    if (n == 3) return 1176900;
    if (seq_is_big()) return a_big(n);
    return a_fixed(n);
}
