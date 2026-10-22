/*
 * A004716  Positions of ones in the binary expansion of log(3)/log(2)-1.
 * https://oeis.org/A004716
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = position of the n-th 1 bit after the binary point of
 *            log_2(3) - 1 = atanh(1/5)/atanh(1/3). Both atanh values are
 *            computed exactly in fixed-point integer arithmetic using their
 *            series, then the quotient is found one bit at a time by long
 *            division
 * Range:     u8 n <= 121, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   bcmp_ge, bshl1, bsub, bzero_p
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/bcmp_ge.h"
#include "../utils/bshl1.h"
#include "../utils/bsub.h"
#include "../utils/bzero_p.h"

static idx_t *pos_cache = NULL;
static idx_t pos_count = 0;

/* out = in / d ; word 0 is the integer part, big-endian fraction words follow */
static void bdiv(uint32_t *out, const uint32_t *in, uint32_t d, size_t W) {
    uint64_t rem = 0;
    size_t i;
    for (i = 0; i < W; i++) {
        uint64_t cur = (rem << 32) | in[i];
        out[i] = (uint32_t)(cur / d);
        rem = cur % d;
    }
}

static void badd(uint32_t *s, const uint32_t *t, size_t W) {
    uint64_t c = 0;
    size_t i = W;
    while (i-- > 0) {
        uint64_t v = (uint64_t)s[i] + t[i] + c;
        s[i] = (uint32_t)v;
        c = v >> 32;
    }
}

/* sum = atanh(1/m) = sum_{k>=0} 1/((2k+1) m^(2k+1)) */
static void atanh_inv(uint32_t *sum, uint32_t m, size_t W) {
    uint32_t *p = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *term = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t k = 0;
    memset(sum, 0, W * sizeof(uint32_t));
    p[0] = 1;
    bdiv(p, p, m, W);
    while (!bzero_p(p, W)) {
        bdiv(term, p, 2 * k + 1, W);
        badd(sum, term, W);
        bdiv(p, p, m * m, W);
        k++;
    }
    free(p);
    free(term);
}

static void compute(idx_t nbits) {
    size_t F = (size_t)nbits + 96;
    size_t W = 1 + (F + 31) / 32;
    uint32_t *A = (uint32_t *)calloc(W, sizeof(uint32_t));
    uint32_t *B = (uint32_t *)calloc(W, sizeof(uint32_t));
    idx_t cap = nbits + 1, cnt = 0, bit;
    idx_t *pos = (idx_t *)malloc(sizeof(idx_t) * cap);
    atanh_inv(A, 5, W);
    atanh_inv(B, 3, W);
    for (bit = 1; bit <= nbits; bit++) {
        bshl1(A, W);
        if (bcmp_ge(A, B, W)) {
            bsub(A, B, W);
            pos[cnt++] = bit;
        }
    }
    free(A);
    free(B);
    free(pos_cache);
    pos_cache = pos;
    pos_count = cnt;
}

seq_t a(idx_t n) {
    if (n > pos_count) {
        idx_t bits = 3 * n + 64;
        for (;;) {
            compute(bits);
            if (pos_count >= n) break;
            bits *= 2;
        }
    }
    return (seq_t)pos_cache[n - 1];
}
