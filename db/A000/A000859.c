/*
 * A000859  Number of n-input 2-output switching networks under action of S(n)
 *          and complementing group C(2,2) on inputs and outputs.
 * https://oeis.org/A000859
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   Burnside's lemma over G = B_n x B_2, where B_n (S(n) with input
 *            complementing) acts on {0,1}^n and B_2 (output permutation and
 *            complementing, order 8) acts on {0,1}^2: a(n) = (1/(8*2^n*n!)) *
 *            Sum_{g in B_n} Sum_{h in B_2} Prod_{cycles c of g on {0,1}^n}
 *            #{v : h^{|c|} v = v}. The sum over g runs o...
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a859_gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/a859_gcd.h"

typedef struct { uint64_t len, cnt; } a859_cyc;

static a859_cyc *a859_base[64];
static int a859_base_n[64];

static uint32_t *a859_big;
static size_t a859_L;
static int a859_n;
static uint64_t a859_fact;

static void a859_push(a859_cyc *lst, int *n, uint64_t len, uint64_t cnt) {
    int i;
    for (i = 0; i < *n; i++)
        if (lst[i].len == len) { lst[i].cnt += cnt; return; }
    lst[*n].len = len;
    lst[*n].cnt = cnt;
    (*n)++;
}

static void a859_build_base(int t) {
    int k = t / 2 + 1, neg = t & 1;
    uint64_t N = (uint64_t)1 << k, mask = N - 1, x;
    char *vis = (char *)calloc((size_t)N, 1);
    a859_cyc *lst = (a859_cyc *)malloc(sizeof(a859_cyc) * (2 * (size_t)k + 2));
    int n = 0;
    for (x = 0; x < N; x++) {
        uint64_t y = x, len = 0;
        if (vis[x]) continue;
        while (!vis[y]) {
            vis[y] = 1;
            len++;
            y = ((y << 1) | (y >> (k - 1))) & mask;
            if (neg) y ^= 1;
        }
        a859_push(lst, &n, len, 1);
    }
    /* convert point counts into cycle counts */
    {
        int i;
        for (i = 0; i < n; i++) lst[i].cnt = lst[i].cnt;
    }
    free(vis);
    a859_base[t] = lst;
    a859_base_n[t] = n;
}

static a859_cyc *a859_combine(const a859_cyc *A, int na, const a859_cyc *B, int nb, int *nout) {
    a859_cyc *R = (a859_cyc *)malloc(sizeof(a859_cyc) * ((size_t)na * nb + 1));
    int n = 0, i, j;
    for (i = 0; i < na; i++)
        for (j = 0; j < nb; j++) {
            uint64_t g = a859_gcd(A[i].len, B[j].len);
            a859_push(R, &n, A[i].len / g * B[j].len, A[i].cnt * B[j].cnt * g);
        }
    *nout = n;
    return R;
}

static void a859_add_at(uint64_t v, uint64_t pos) {
    size_t i = (size_t)(pos / 32);
    int b = (int)(pos % 32);
    uint64_t lo = v << b;
    uint64_t hi = b ? (v >> (64 - b)) : 0;
    uint64_t parts[3], carry = 0;
    int j;
    parts[0] = lo & 0xffffffffu;
    parts[1] = lo >> 32;
    parts[2] = hi;
    for (j = 0; j < 3; j++) {
        uint64_t s = (uint64_t)a859_big[i] + parts[j] + carry;
        a859_big[i] = (uint32_t)s;
        carry = s >> 32;
        i++;
    }
    while (carry) {
        uint64_t s = (uint64_t)a859_big[i] + carry;
        a859_big[i] = (uint32_t)s;
        carry = s >> 32;
        i++;
    }
}

static void a859_leaf(uint64_t Z, int ell, const a859_cyc *S, int ns) {
    uint64_t cs = a859_fact / Z;
    uint64_t sh = (uint64_t)(a859_n - ell);
    uint64_t C = 0, Codd = 0;
    int all4 = 1, i;
    for (i = 0; i < ns; i++) {
        C += S[i].cnt;
        if (S[i].len & 1) Codd += S[i].cnt;
        if (S[i].len % 4) all4 = 0;
    }
    a859_add_at(cs, sh + 2 * C);
    if (Codd == 0) a859_add_at(3 * cs, sh + 2 * C);
    a859_add_at(2 * cs, sh + Codd + 2 * (C - Codd));
    if (all4) a859_add_at(2 * cs, sh + 2 * C);
}

static void a859_rec(int t, int rem, uint64_t Z, int ell, a859_cyc *S, int ns) {
    int k, m, curn;
    uint64_t Zc;
    a859_cyc *cur;
    if (rem == 0) { a859_leaf(Z, ell, S, ns); return; }
    if (t >= 2 * a859_n) return;
    k = t / 2 + 1;
    if (k > rem) return;
    if (!a859_base[t]) a859_build_base(t);
    a859_rec(t + 1, rem, Z, ell, S, ns);
    cur = S; curn = ns; Zc = Z;
    for (m = 1; m * k <= rem; m++) {
        int nn;
        a859_cyc *nxt = a859_combine(cur, curn, a859_base[t], a859_base_n[t], &nn);
        if (cur != S) free(cur);
        cur = nxt; curn = nn;
        Zc *= (uint64_t)k * (uint64_t)m;
        a859_rec(t + 1, rem - m * k, Zc, ell + m, cur, curn);
    }
    if (cur != S) free(cur);
}

seq_t a(idx_t n) {
    int N = (int)n, i, j, k;
    size_t L, top;
    a859_cyc one;
    uint64_t s;
    seq_t r = (seq_t)0;
    if (n < 1 || n > 20) return (seq_t)0;
    a859_n = N;
    a859_fact = 1;
    for (i = 2; i <= N; i++) a859_fact *= (uint64_t)i;
    L = (size_t)(((uint64_t)N + ((uint64_t)2 << N) + 128) / 32 + 8);
    a859_L = L;
    a859_big = (uint32_t *)calloc(L, sizeof(uint32_t));
    one.len = 1; one.cnt = 1;
    a859_rec(0, N, 1, 0, &one, 1);

    /* shift right by n+3 */
    s = (uint64_t)N + 3;
    {
        size_t w = (size_t)(s / 32), idx;
        int b = (int)(s % 32);
        for (idx = 0; idx < L; idx++) {
            uint64_t lo = (idx + w < L) ? a859_big[idx + w] : 0;
            uint64_t hi = (idx + w + 1 < L) ? a859_big[idx + w + 1] : 0;
            uint64_t v = (lo | (hi << 32)) >> b;
            a859_big[idx] = (uint32_t)v;
        }
    }
    /* divide by n! */
    for (k = 2; k <= N; k++) {
        uint64_t rem = 0;
        size_t idx = L;
        while (idx > 0) {
            uint64_t cur;
            idx--;
            cur = (rem << 32) | a859_big[idx];
            a859_big[idx] = (uint32_t)(cur / (uint64_t)k);
            rem = cur % (uint64_t)k;
        }
    }
    top = L;
    while (top > 0 && a859_big[top - 1] == 0) top--;
    while (top > 0) {
        uint32_t limb;
        top--;
        limb = a859_big[top];
        for (j = 3; j >= 0; j--) {
            r = (seq_t)(r << 8);
            r += (seq_t)((limb >> (8 * j)) & 255u);
        }
    }
    free(a859_big);
    a859_big = NULL;
    return r;
}
