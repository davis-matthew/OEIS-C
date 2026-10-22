/*
 * A007139  Number of unlabeled bicolored bipartite graphs on 2n nodes having
 *          n nodes of each color with no edges between vertices of the same
 *          color and allowing the color classes to be interchanged.
 * https://oeis.org/A007139
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (A002724(n) + A122082(n))/2, computed by Burnside's lemma
 *            over (S_n x S_n) wr Z_2: a(n) = Sum_{lambda |- n} (F(lambda) +
 *            2^c(lambda)) / (2*z(lambda)), where F(lambda) = [x^n] exp(Sum_k
 *            2^(Sum_i gcd(lambda_i,k)) x^k/k) (Euler transform), c(lambda) =
 *            Sum_i ceiling(lambda_i/2) + Sum_{i<j} gcd...
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 10, u128 n <= 14,
 *            big any n
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/gcd_u64.h"

static int bn_norm(const uint32_t *x, int len) {
    while (len > 0 && x[len - 1] == 0) len--;
    return len;
}

/* acc += src << sh; acc must be zero above alen */
static int bn_add_shifted(uint32_t *acc, int alen, const uint32_t *src, int sl, unsigned sh) {
    int ws = (int)(sh / 32);
    unsigned bs = sh % 32;
    uint64_t carry = 0;
    int i, idx = ws;
    for (i = 0; i <= sl; i++) {
        uint32_t cur = i < sl ? src[i] : 0;
        uint32_t prev = i > 0 ? src[i - 1] : 0;
        uint32_t piece = bs ? (uint32_t)((cur << bs) | (prev >> (32 - bs))) : cur;
        uint64_t s = (uint64_t)acc[idx] + piece + carry;
        acc[idx] = (uint32_t)s;
        carry = s >> 32;
        idx++;
    }
    while (carry) {
        uint64_t s = (uint64_t)acc[idx] + carry;
        acc[idx] = (uint32_t)s;
        carry = s >> 32;
        idx++;
    }
    if (idx > alen) alen = idx;
    return bn_norm(acc, alen);
}

static int bn_mul_small(uint32_t *x, int len, uint32_t k) {
    uint64_t c = 0;
    int i;
    for (i = 0; i < len; i++) {
        uint64_t t = (uint64_t)x[i] * k + c;
        x[i] = (uint32_t)t;
        c = t >> 32;
    }
    if (c) x[len++] = (uint32_t)c;
    return bn_norm(x, len);
}

static int bn_div_small(uint32_t *x, int len, uint32_t k) {
    uint64_t r = 0;
    int i;
    for (i = len - 1; i >= 0; i--) {
        uint64_t cur = (r << 32) | x[i];
        x[i] = (uint32_t)(cur / k);
        r = cur % k;
    }
    return bn_norm(x, len);
}

seq_t a(idx_t n) {
    uint64_t lg = 1, i, j, m, kk;
    int L, k, tlen, xlen;
    uint32_t one = 1;
    uint32_t **b, *blenbuf, *tmp, *total;
    int *blen;
    uint64_t *p, *e;
    seq_t result = 0;

    while (((uint64_t)1 << lg) <= n + 1 && lg < 63) lg++;
    L = (int)((n * n + n * lg + 64) / 32) + 6;

    b = (uint32_t **)malloc((size_t)(n + 1) * sizeof(uint32_t *));
    blenbuf = (uint32_t *)calloc((size_t)(n + 1) * (size_t)L, sizeof(uint32_t));
    blen = (int *)calloc((size_t)(n + 1), sizeof(int));
    tmp = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    total = (uint32_t *)calloc((size_t)L, sizeof(uint32_t));
    p = (uint64_t *)calloc((size_t)(n + 1), sizeof(uint64_t));
    e = (uint64_t *)calloc((size_t)(n + 1), sizeof(uint64_t));
    for (i = 0; i <= n; i++) b[i] = blenbuf + i * (size_t)L;
    tlen = 0;

    k = 0;
    if (n > 0) { p[0] = n; k = 1; }

    for (;;) {
        uint64_t c = 0;
        /* exponents for the Euler transform */
        for (kk = 1; kk <= n; kk++) {
            uint64_t s = 0;
            for (j = 0; j < (uint64_t)k; j++) s += gcd_u64(p[j], kk);
            e[kk] = s;
        }
        memset(b[0], 0, (size_t)L * sizeof(uint32_t));
        b[0][0] = 1;
        blen[0] = 1;
        for (m = 1; m <= n; m++) {
            int len = 0;
            memset(b[m], 0, (size_t)L * sizeof(uint32_t));
            for (kk = 1; kk <= m; kk++)
                len = bn_add_shifted(b[m], len, b[m - kk], blen[m - kk], (unsigned)e[kk]);
            blen[m] = bn_div_small(b[m], len, (uint32_t)m);
        }
        /* swap-coset exponent */
        for (j = 0; j < (uint64_t)k; j++) {
            c += (p[j] + 1) / 2;
            for (i = j + 1; i < (uint64_t)k; i++) c += gcd_u64(p[i], p[j]);
        }
        memset(tmp, 0, (size_t)L * sizeof(uint32_t));
        memcpy(tmp, b[n], (size_t)blen[n] * sizeof(uint32_t));
        xlen = bn_add_shifted(tmp, blen[n], &one, 1, (unsigned)c);
        /* multiply by n!/z(lambda) */
        for (i = 2; i <= n; i++) xlen = bn_mul_small(tmp, xlen, (uint32_t)i);
        j = 0;
        while (j < (uint64_t)k) {
            uint64_t v = p[j], mult = 0;
            while (j < (uint64_t)k && p[j] == v) { j++; mult++; }
            for (i = 1; i <= mult; i++) {
                xlen = bn_div_small(tmp, xlen, (uint32_t)v);
                xlen = bn_div_small(tmp, xlen, (uint32_t)i);
            }
        }
        tlen = bn_add_shifted(total, tlen, tmp, xlen, 0);

        /* next partition */
        {
            int jj = k - 1;
            uint64_t rem = 0, v;
            while (jj >= 0 && p[jj] == 1) { rem++; jj--; }
            if (jj < 0) break;
            p[jj]--;
            rem++;
            v = p[jj];
            k = jj + 1;
            while (rem > v) { p[k++] = v; rem -= v; }
            if (rem > 0) p[k++] = rem;
        }
    }

    tlen = bn_div_small(total, tlen, 2);
    for (i = 2; i <= n; i++) tlen = bn_div_small(total, tlen, (uint32_t)i);

    for (k = tlen - 1; k >= 0; k--) {
        int sh;
        for (sh = 24; sh >= 0; sh -= 8) {
            result <<= 8;
            result += (idx_t)((total[k] >> sh) & 0xFFu);
        }
    }

    free(b);
    free(blenbuf);
    free(blen);
    free(tmp);
    free(total);
    free(p);
    free(e);
    return result;
}
