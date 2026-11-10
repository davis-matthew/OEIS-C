/*
 * A218788  a(n) = A014486-index for the n-th tendril of infinite beanstalk
 *          (A213730(n)), with the "lesser numbers to the right side"
 *          construction.
 * https://oeis.org/A218788
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014486-index of the finite binary tree (internal nodes
 *            only, larger child to the left) of the n-th tendril root
 *            A213730(n), where children of m are 2k, 2k+1 with m = 2k -
 *            popcount(k) (A011371 parent map); the trunk is found by lockstep
 *            search for the child whose subtree dies out
 * Range:     u8 n <= 212, u16 n <= 388, u32 n <= 702, u64 n <= 2465,
 *            u128 n <= 8687, big any n
 * Validated: standard, pass, 102 terms through n = 8498 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

typedef uint64_t bs_u64;

static int bs_kids(bs_u64 m, bs_u64 *lo)
{
    bs_u64 l = 0, h = m;
    while (l <= h) {
        bs_u64 k = l + (h - l) / 2;
        bs_u64 g = 2 * k - (bs_u64)__builtin_popcountll(k);
        if (g == m) { *lo = 2 * k; return 1; }
        if (g < m) l = k + 1;
        else { if (k == 0) break; h = k - 1; }
    }
    return 0;
}

static size_t bs_expand(const bs_u64 *in, size_t n, bs_u64 **out, size_t *cap)
{
    size_t c = 0, i;
    for (i = 0; i < n; i++) {
        bs_u64 lo;
        if (bs_kids(in[i], &lo)) {
            if (c + 2 > *cap) {
                *cap = (*cap + 2) * 2;
                *out = (bs_u64 *)realloc(*out, *cap * sizeof(bs_u64));
            }
            (*out)[c++] = lo;
            (*out)[c++] = lo + 1;
        }
    }
    return c;
}

/* returns 1 if the subtree of a dies out before that of b */
static int bs_a_finite(bs_u64 a, bs_u64 b)
{
    size_t ca = 4, cb = 4, na = 1, nb = 1;
    bs_u64 *fa = (bs_u64 *)malloc(ca * sizeof(bs_u64));
    bs_u64 *fb = (bs_u64 *)malloc(cb * sizeof(bs_u64));
    bs_u64 *ta = (bs_u64 *)malloc(ca * sizeof(bs_u64));
    bs_u64 *tb = (bs_u64 *)malloc(cb * sizeof(bs_u64));
    size_t cta = ca, ctb = cb;
    int res;
    fa[0] = a; fb[0] = b;
    for (;;) {
        bs_u64 *t;
        size_t tc;
        if (na == 0) { res = 1; break; }
        if (nb == 0) { res = 0; break; }
        na = bs_expand(fa, na, &ta, &cta);
        nb = bs_expand(fb, nb, &tb, &ctb);
        t = fa; fa = ta; ta = t; tc = ca; ca = cta; cta = tc;
        t = fb; fb = tb; tb = t; tc = cb; cb = ctb; ctb = tc;
    }
    free(fa); free(fb); free(ta); free(tb);
    return res;
}

static size_t bs_count(bs_u64 node)
{
    bs_u64 lo;
    if (!bs_kids(node, &lo)) return 0;
    return 1 + bs_count(lo + 1) + bs_count(lo);
}

static void bs_build(bs_u64 node, char *buf, size_t *pos)
{
    bs_u64 lo;
    if (!bs_kids(node, &lo)) return;
    buf[(*pos)++] = 1;
    bs_build(lo + 1, buf, pos);
    buf[(*pos)++] = 0;
    bs_build(lo, buf, pos);
}

static seq_t bs_index(bs_u64 root)
{
    size_t s = bs_count(root);
    size_t L = 2 * s, W = L + 2, r, h, i, k;
    seq_t res = 0;
    seq_t *T;
    char *buf;
    size_t pos = 0;
    if (s == 0) return (seq_t)0;
    T = mem_new(seq_t, (L + 1) * W);
    buf = (char *)malloc(L + 1);
    bs_build(root, buf, &pos);
    T[0] = (seq_t)1;
    for (r = 1; r <= L; r++)
        for (h = 0; h <= L; h++) {
            seq_t v = T[(r - 1) * W + h + 1];
            if (h > 0) v += T[(r - 1) * W + h - 1];
            T[r * W + h] = v;
        }
    for (k = 0; k < s; k++) res += T[(2 * k) * W];
    h = 0;
    for (i = 0; i < L; i++) {
        size_t rem = L - i - 1;
        if (buf[i]) {
            if (h > 0) res += T[rem * W + h - 1];
            h++;
        } else {
            h--;
        }
    }
    mem_free(T);
    free(buf);
    return res;
}

static seq_t *bs_cache = NULL;
static size_t bs_len = 0, bs_cap = 0;
static bs_u64 bs_trunk = 1;

seq_t a(idx_t n)
{
    while (bs_len < (size_t)n) {
        bs_u64 lo, tend;
        if (bs_len == bs_cap) {
            size_t nc = bs_cap ? bs_cap * 2 : 64;
            seq_t *p = mem_resize(seq_t, bs_cache, nc);
            if (!p) return (seq_t)0;
            bs_cache = p;
            bs_cap = nc;
        }
        bs_kids(bs_trunk, &lo);
        if (bs_a_finite(lo, lo + 1)) { tend = lo; bs_trunk = lo + 1; }
        else { tend = lo + 1; bs_trunk = lo; }
        bs_cache[bs_len++] = bs_index(tend);
    }
    return bs_cache[(size_t)n - 1];
}
