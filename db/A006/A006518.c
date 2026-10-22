/*
 * A006518  Continued fraction for Sum_{k >= 2} 2^(-Fibonacci(k)).
 * https://oeis.org/A006518
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr
 * Formula:   continued fraction expansion of x = Sum_{k>=2} 2^(-F(k)). Exact
 *            method: x lies between p/2^N and (p+2)/2^N, where N = F(m) and
 *            p/2^N = Sum_{k=2..m} 2^(-F(k)). Run the Euclidean algorithm on
 *            both bounds and keep the partial quotients they share. Increase
 *            m until there are enough terms
 * Range:     u8 n <= 12, u16 n <= 18, u32 n <= 34, u64 n <= 46, u128 n <= 86,
 *            big any n
 * Helpers:   bn_cmp_sh, bn_sub_sh, sh_limb, to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <string.h>
#include <stdint.h>
#include "../utils/bn_cmp_sh.h"
#include "../utils/bn_sub_sh.h"
#include "../utils/sh_limb.h"
#include "../utils/to_seq.h"

static size_t bn_norm(const uint32_t *a, size_t len) {
    while (len && a[len - 1] == 0) len--;
    return len;
}

static size_t bn_bits(const uint32_t *a, size_t len) {
    size_t b;
    uint32_t t;
    if (!len) return 0;
    t = a[len - 1];
    b = (len - 1) * 32;
    while (t) { b++; t >>= 1; }
    return b;
}

/* a := a mod b, quotient limbs into q; returns normalized quotient length */
static size_t bn_divmod(uint32_t *a, size_t *alen, const uint32_t *b, size_t blen, uint32_t *q) {
    size_t ab = bn_bits(a, *alen), bb = bn_bits(b, blen), s, ql, j;
    if (ab < bb) return 0;
    s = ab - bb;
    ql = s / 32 + 1;
    memset(q, 0, ql * sizeof(uint32_t));
    for (j = s + 1; j-- > 0;) {
        if (bn_cmp_sh(a, *alen, b, blen, j) >= 0) {
            bn_sub_sh(a, *alen, b, blen, j);
            *alen = bn_norm(a, *alen);
            q[j / 32] |= (uint32_t)1 << (j % 32);
        }
    }
    return bn_norm(q, ql);
}

/* terms stored as raw limbs: term t occupies pool[off[t] .. off[t+1]) */
static uint32_t *g_pool = NULL;
static size_t *g_off = NULL;
static idx_t nterms = 0;

static void compute(int m) {
    uint64_t F[100];
    size_t N, L, a1l, b1l, a2l, b2l, i;
    uint32_t *p1, *q1, *p2, *q2, *qa, *qb, *a1, *b1, *a2, *b2, *tp;
    uint32_t *pool;
    size_t *off;
    size_t pcap = 256, plen = 0, ocap = 64;
    idx_t cnt = 0;
    int k;
    uint64_t carry;

    F[0] = 0; F[1] = 1;
    for (k = 2; k <= m; k++) F[k] = F[k - 1] + F[k - 2];
    N = (size_t)F[m];
    L = N / 32 + 2;
    p1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    q1 = (uint32_t *)calloc(L, sizeof(uint32_t));
    p2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    q2 = (uint32_t *)calloc(L, sizeof(uint32_t));
    qa = (uint32_t *)calloc(L, sizeof(uint32_t));
    qb = (uint32_t *)calloc(L, sizeof(uint32_t));
    pool = (uint32_t *)malloc(pcap * sizeof(uint32_t));
    off = (size_t *)malloc(ocap * sizeof(size_t));
    off[0] = 0;

    for (k = 2; k <= m; k++) {
        size_t e = N - (size_t)F[k];
        p1[e / 32] |= (uint32_t)1 << (e % 32);
    }
    q1[N / 32] |= (uint32_t)1 << (N % 32);
    memcpy(q2, q1, L * sizeof(uint32_t));
    memcpy(p2, p1, L * sizeof(uint32_t));
    carry = 2;
    for (i = 0; i < L && carry; i++) {
        uint64_t t = (uint64_t)p2[i] + carry;
        p2[i] = (uint32_t)t;
        carry = t >> 32;
    }

    a1 = p1; b1 = q1; a2 = p2; b2 = q2;
    a1l = bn_norm(a1, L); b1l = bn_norm(b1, L);
    a2l = bn_norm(a2, L); b2l = bn_norm(b2, L);

    while (b1l && b2l) {
        size_t l1 = bn_divmod(a1, &a1l, b1, b1l, qa);
        size_t l2 = bn_divmod(a2, &a2l, b2, b2l, qb);
        if (l1 != l2 || memcmp(qa, qb, l1 * sizeof(uint32_t)) != 0) break;
        if (a1l == 0 || a2l == 0) break;
        if ((size_t)cnt + 2 > ocap) {
            ocap *= 2;
            off = (size_t *)realloc(off, ocap * sizeof(size_t));
        }
        while (plen + l1 > pcap) {
            pcap *= 2;
            pool = (uint32_t *)realloc(pool, pcap * sizeof(uint32_t));
        }
        if (l1) memcpy(pool + plen, qa, l1 * sizeof(uint32_t));
        plen += l1;
        cnt++;
        off[cnt] = plen;
        tp = a1; a1 = b1; b1 = tp; i = a1l; a1l = b1l; b1l = i;
        tp = a2; a2 = b2; b2 = tp; i = a2l; a2l = b2l; b2l = i;
    }

    if (cnt > nterms) {
        free(g_pool);
        free(g_off);
        g_pool = pool;
        g_off = off;
        nterms = cnt;
    } else {
        free(pool);
        free(off);
    }
    free(p1); free(q1); free(p2); free(q2); free(qa); free(qb);
}

seq_t a(idx_t n) {
    static int m = 8;
    while (n >= nterms && m < 99) {
        compute(m);
        m++;
    }
    if (n >= nterms) return 0;
    return to_seq(g_pool + g_off[n], g_off[n + 1] - g_off[n]);
}
