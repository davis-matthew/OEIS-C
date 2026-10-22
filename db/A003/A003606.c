/*
 * A003606  a(n) = number of types of conjugacy classes in GL(n,q) (this is
 *          independent of q).
 * https://oeis.org/A003606
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(0) = 1, a(n) = (1/n) * Sum_{k=0..n-1} a(k)*g(n-k), where g(m)
 *            = Sum_{d|m} d*A000041(d)*A000203(m/d)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 34, u64 n <= 80, u128 n <= 187,
 *            big any n
 * Helpers:   zalloc_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include "../utils/zalloc_n.h"

static int W_bits = -1;          /* 0 = arbitrary precision, else width in bits */
static seq_t *memo = NULL;       /* a(0..done) */
static seq_t *pp = NULL, *sg = NULL, *gg = NULL;
static idx_t cap = 0, done = 0;

static void detect_width(void) {
    seq_t x = 1;
    int w = 0;
    while (w < 300 && x != 0) { x *= 2; w++; }
    W_bits = (x == 0) ? w : 0;
}

static void ensure(idx_t N) {
    if (N < cap) return;
    idx_t nc = cap ? cap : 64;
    while (nc <= N) nc *= 2;
    seq_t *nm = zalloc_n(nc);
    if (memo) {
        for (idx_t i = 0; i <= done; i++) nm[i] = memo[i];
        free(memo);
    } else {
        nm[0] = 1;
        done = 0;
    }
    memo = nm;
    if (pp) free(pp);
    if (sg) free(sg);
    if (gg) free(gg);
    pp = zalloc_n(nc);
    sg = zalloc_n(nc);
    gg = zalloc_n(nc);
    /* partitions via pentagonal number theorem */
    pp[0] = 1;
    for (idx_t m = 1; m < nc; m++) {
        seq_t pos = 0, neg = 0;
        for (idx_t k = 1;; k++) {
            idx_t g1 = k * (3 * k - 1) / 2;
            if (g1 > m) break;
            idx_t g2 = k * (3 * k + 1) / 2;
            if (k & 1) {
                pos += pp[m - g1];
                if (g2 <= m) pos += pp[m - g2];
            } else {
                neg += pp[m - g1];
                if (g2 <= m) neg += pp[m - g2];
            }
        }
        pp[m] = pos - neg;
    }
    /* sigma */
    for (idx_t d = 1; d < nc; d++)
        for (idx_t m = d; m < nc; m += d) sg[m] += d;
    /* g(m) = Sum_{d|m} d*p(d)*sigma(m/d) */
    for (idx_t d = 1; d < nc; d++) {
        seq_t c = (seq_t)d * pp[d];
        for (idx_t q = 1; d * q < nc; q++) gg[d * q] += c * sg[q];
    }
    cap = nc;
}

static void mul_wide(seq_t x, seq_t y, seq_t *hi, seq_t *lo) {
    int H = W_bits / 2;
    seq_t M = (seq_t)(((seq_t)1 << H) - 1);
    seq_t x0 = (seq_t)(x & M), x1 = (seq_t)(x >> H);
    seq_t y0 = (seq_t)(y & M), y1 = (seq_t)(y >> H);
    seq_t p00 = (seq_t)(x0 * y0), p01 = (seq_t)(x0 * y1);
    seq_t p10 = (seq_t)(x1 * y0), p11 = (seq_t)(x1 * y1);
    seq_t mid = (seq_t)((seq_t)(p00 >> H) + (seq_t)(p01 & M) + (seq_t)(p10 & M));
    *lo = (seq_t)((seq_t)(p00 & M) | (seq_t)(mid << H));
    *hi = (seq_t)(p11 + (seq_t)(p01 >> H) + (seq_t)(p10 >> H) + (seq_t)(mid >> H));
}

seq_t a(idx_t n) {
    if (W_bits < 0) detect_width();
    ensure(n);
    for (idx_t i = done + 1; i <= n; i++) {
        if (W_bits == 0) {
            seq_t S = 0;
            for (idx_t k = 0; k < i; k++) S += memo[k] * gg[i - k];
            memo[i] = S / (seq_t)i;
        } else {
            seq_t shi = 0, slo = 0;
            for (idx_t k = 0; k < i; k++) {
                seq_t h, l;
                mul_wide(memo[k], gg[i - k], &h, &l);
                slo = (seq_t)(slo + l);
                if (slo < l) shi = (seq_t)(shi + 1);
                shi = (seq_t)(shi + h);
            }
            int e = 0;
            idx_t o = i;
            while ((o & 1) == 0) { o >>= 1; e++; }
            seq_t q;
            if (e == 0) q = slo;
            else if (e < W_bits) q = (seq_t)((seq_t)(slo >> e) | (seq_t)(shi << (W_bits - e)));
            else if (e - W_bits < W_bits) q = (seq_t)(shi >> (e - W_bits));
            else q = 0;
            seq_t ov = (seq_t)o, inv = ov;
            for (int t = 0; t < 8; t++) inv = (seq_t)(inv * (seq_t)((seq_t)2 - (seq_t)(ov * inv)));
            memo[i] = (seq_t)(q * inv);
        }
    }
    if (n > done) done = n;
    return memo[n];
}
