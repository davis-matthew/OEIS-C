/*
 * A003085  Number of weakly connected digraphs with n unlabeled nodes.
 * https://oeis.org/A003085
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, changed
 * Formula:   a(n) = (1/n)*Sum_{d|n} mu(n/d)*c(d), c(n) = n*b(n) -
 *            Sum_{k=1..n-1} c(k)*b(n-k) (inverse Euler transform of b =
 *            A000273), with b(n) = Sum_{partitions L of n} 2^(Sum_i (L_i - 1)
 *            + 2*Sum_{i<j} gcd(L_i,L_j)) / z(L) (Burnside / cycle index count
 *            of unlabeled digraphs)
 * Range:     u8 n <= 4, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   q3085_bl, q3085_fdiv, q3085_fmul, q3085_mu
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/q3085_bl.h"
#include "../utils/q3085_fdiv.h"
#include "../utils/q3085_fmul.h"
#include "../utils/q3085_mu.h"

typedef struct { uint32_t *d; size_t n; } q3085_bn;

static q3085_bn q3085_new(size_t n) {
    q3085_bn r;
    r.n = n;
    r.d = (uint32_t *)calloc(n ? n : 1, sizeof(uint32_t));
    return r;
}
static void q3085_trim(q3085_bn *x) { while (x->n && x->d[x->n - 1] == 0) x->n--; }

static q3085_bn q3085_add(q3085_bn a, q3085_bn b) {
    size_t n = (a.n > b.n ? a.n : b.n) + 1, t;
    q3085_bn r = q3085_new(n);
    uint64_t c = 0;
    for (t = 0; t < n; t++) {
        c += (t < a.n ? a.d[t] : 0);
        c += (t < b.n ? b.d[t] : 0);
        r.d[t] = (uint32_t)c;
        c >>= 32;
    }
    q3085_trim(&r);
    return r;
}
static q3085_bn q3085_sub(q3085_bn a, q3085_bn b) { /* a >= b */
    q3085_bn r = q3085_new(a.n);
    int64_t br = 0;
    size_t t;
    for (t = 0; t < a.n; t++) {
        int64_t v = (int64_t)a.d[t] - (t < b.n ? (int64_t)b.d[t] : 0) - br;
        if (v < 0) { v += ((int64_t)1 << 32); br = 1; } else br = 0;
        r.d[t] = (uint32_t)v;
    }
    q3085_trim(&r);
    return r;
}
static q3085_bn q3085_mul(q3085_bn a, q3085_bn b) {
    q3085_bn r = q3085_new(a.n + b.n);
    size_t i, j;
    for (i = 0; i < a.n; i++) {
        uint64_t c = 0;
        for (j = 0; j < b.n; j++) {
            uint64_t t = (uint64_t)a.d[i] * b.d[j] + r.d[i + j] + c;
            r.d[i + j] = (uint32_t)t;
            c = t >> 32;
        }
        r.d[i + b.n] = (uint32_t)c;
    }
    q3085_trim(&r);
    return r;
}
static q3085_bn q3085_muls(q3085_bn a, uint32_t m) {
    q3085_bn r = q3085_new(a.n + 1);
    uint64_t c = 0;
    size_t t;
    for (t = 0; t < a.n; t++) {
        c += (uint64_t)a.d[t] * m;
        r.d[t] = (uint32_t)c;
        c >>= 32;
    }
    r.d[a.n] = (uint32_t)c;
    q3085_trim(&r);
    return r;
}
static void q3085_divs(q3085_bn *a, uint32_t m) {
    uint64_t rem = 0;
    size_t t;
    for (t = a->n; t-- > 0;) {
        uint64_t cur = (rem << 32) | a->d[t];
        a->d[t] = (uint32_t)(cur / m);
        rem = cur % m;
    }
    q3085_trim(a);
}

static int q3085_m;
static size_t q3085_CL;
static uint32_t *q3085_coef, *q3085_cs;
static size_t *q3085_len;
static int *q3085_parts;
static int q3085_k;
static unsigned char *q3085_gt;

static void q3085_rec(int r, int maxp, int lastj, long e, int depth) {
    uint32_t *cur = q3085_cs + (size_t)depth * q3085_CL;
    size_t L = q3085_len[depth];
    int top = maxp < r ? maxp : r;
    int i, t;
    for (i = top; i >= 2; i--) {
        int j = (q3085_k > 0 && q3085_parts[q3085_k - 1] == i) ? lastj + 1 : 1;
        long G = 0;
        uint32_t *nx = cur + q3085_CL;
        size_t nl = L;
        uint64_t f = 1;
        int x;
        for (t = 0; t < q3085_k; t++) G += q3085_gt[i * (q3085_m + 1) + q3085_parts[t]];
        memcpy(nx, cur, L * sizeof(uint32_t));
        for (x = r; x > r - i; x--) {
            if (f * (uint64_t)x > 0xFFFFFFFFull) { nl = q3085_fmul(nx, nl, (uint32_t)f); f = (uint64_t)x; }
            else f *= (uint64_t)x;
        }
        nl = q3085_fmul(nx, nl, (uint32_t)f);
        nl = q3085_fdiv(nx, nl, (uint32_t)(i * j));
        q3085_len[depth + 1] = nl;
        q3085_parts[q3085_k++] = i;
        q3085_rec(r - i, i, j, e + (i - 1) + 2 * G, depth + 1);
        q3085_k--;
    }
    {
        /* fill the rest with fixed points */
        long ne = e + 2L * r * q3085_k + (long)r * (r - 1);
        uint32_t *dst = q3085_coef + (size_t)ne * q3085_CL;
        uint64_t c = 0;
        size_t u;
        for (u = 0; u < L; u++) {
            c += (uint64_t)dst[u] + cur[u];
            dst[u] = (uint32_t)c;
            c >>= 32;
        }
        while (c && u < q3085_CL) {
            c += dst[u];
            dst[u] = (uint32_t)c;
            c >>= 32;
            u++;
        }
    }
}

/* A000273(m): unlabeled digraphs on m nodes */
static q3085_bn q3085_t273(int m) {
    long bits = 32, E = (long)m * (m - 1), e;
    size_t CL, TL, t;
    int x, y, k;
    uint32_t *T;
    q3085_bn res;
    for (x = 2; x <= m; x++) bits += q3085_bl((unsigned)x);
    bits += 2 * q3085_bl((unsigned)m);
    CL = (size_t)(bits / 32 + 2);
    q3085_m = m;
    q3085_CL = CL;
    q3085_coef = (uint32_t *)calloc((size_t)(E + 1) * CL, sizeof(uint32_t));
    q3085_cs = (uint32_t *)calloc((size_t)(m + 2) * CL, sizeof(uint32_t));
    q3085_len = (size_t *)calloc((size_t)(m + 2), sizeof(size_t));
    q3085_parts = (int *)calloc((size_t)(m + 2), sizeof(int));
    q3085_gt = (unsigned char *)calloc((size_t)(m + 1) * (m + 1), 1);
    for (x = 0; x <= m; x++)
        for (y = 0; y <= m; y++) {
            int p = x, q = y;
            while (q) { int r = p % q; p = q; q = r; }
            q3085_gt[x * (m + 1) + y] = (unsigned char)p;
        }
    q3085_cs[0] = 1;
    q3085_len[0] = 1;
    q3085_k = 0;
    q3085_rec(m, m, 0, 0, 0);

    TL = (size_t)(E / 32) + CL + 3;
    T = (uint32_t *)calloc(TL, sizeof(uint32_t));
    for (e = 0; e <= E; e++) {
        uint32_t *c = q3085_coef + (size_t)e * CL;
        size_t off = (size_t)(e >> 5), idx;
        unsigned sh = (unsigned)(e & 31);
        uint64_t carry = 0, prev = 0;
        int nz = 0;
        for (t = 0; t < CL; t++) if (c[t]) { nz = 1; break; }
        if (!nz) continue;
        for (t = 0; t <= CL; t++) {
            uint64_t v = t < CL ? ((uint64_t)c[t] << sh) : 0;
            uint32_t w = (uint32_t)v | (uint32_t)(prev >> 32);
            carry += (uint64_t)T[off + t] + w;
            T[off + t] = (uint32_t)carry;
            carry >>= 32;
            prev = v;
        }
        idx = off + CL + 1;
        while (carry && idx < TL) {
            carry += T[idx];
            T[idx] = (uint32_t)carry;
            carry >>= 32;
            idx++;
        }
    }
    for (k = 2; k <= m; k++) {
        uint64_t rem = 0;
        for (t = TL; t-- > 0;) {
            uint64_t cur = (rem << 32) | T[t];
            T[t] = (uint32_t)(cur / (uint64_t)k);
            rem = cur % (uint64_t)k;
        }
    }
    res.d = T;
    res.n = TL;
    q3085_trim(&res);
    free(q3085_coef); free(q3085_cs); free(q3085_len); free(q3085_parts); free(q3085_gt);
    return res;
}

static q3085_bn *q3085_B = NULL, *q3085_C = NULL, *q3085_A = NULL;
static idx_t q3085_done = 0;

seq_t a(idx_t n) {
    seq_t r = 0;
    size_t t;
    int s;
    if (n > q3085_done) {
        idx_t m, k;
        q3085_B = (q3085_bn *)realloc(q3085_B, (size_t)(n + 1) * sizeof(q3085_bn));
        q3085_C = (q3085_bn *)realloc(q3085_C, (size_t)(n + 1) * sizeof(q3085_bn));
        q3085_A = (q3085_bn *)realloc(q3085_A, (size_t)(n + 1) * sizeof(q3085_bn));
        for (m = q3085_done + 1; m <= n; m++) {
            q3085_bn sum = q3085_new(0), tmp, pos = q3085_new(0), neg = q3085_new(0), diff;
            q3085_B[m] = q3085_t273((int)m);
            for (k = 1; k < m; k++) {
                q3085_bn p = q3085_mul(q3085_C[k], q3085_B[m - k]);
                q3085_bn ns = q3085_add(sum, p);
                free(sum.d); free(p.d);
                sum = ns;
            }
            tmp = q3085_muls(q3085_B[m], (uint32_t)m);
            q3085_C[m] = q3085_sub(tmp, sum);
            free(tmp.d); free(sum.d);
            for (k = 1; k <= m; k++) {
                if (m % k) continue;
                {
                    int mu = q3085_mu(m / k);
                    if (mu == 1) { q3085_bn ns = q3085_add(pos, q3085_C[k]); free(pos.d); pos = ns; }
                    else if (mu == -1) { q3085_bn ns = q3085_add(neg, q3085_C[k]); free(neg.d); neg = ns; }
                }
            }
            diff = q3085_sub(pos, neg);
            free(pos.d); free(neg.d);
            q3085_divs(&diff, (uint32_t)m);
            q3085_A[m] = diff;
            q3085_done = m;
        }
    }
    for (t = q3085_A[n].n; t-- > 0;) {
        for (s = 0; s < 4; s++) r = (seq_t)(r * 256);
        r += (idx_t)q3085_A[n].d[t];
    }
    return r;
}
