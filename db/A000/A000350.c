/*
 * A000350  Numbers m such that Fibonacci(m) ends with m.
 * https://oeis.org/A000350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   search. A d-digit m is a term iff F(m) == m (mod 10^d). F(m) mod
 *            10^d depends only on m mod L_d = lcm(pi(10^d), 10^d) (L_1=60,
 *            L_2=300, L_d=3*10^d for d>=3), so the solutions mod L_d are
 *            lifted one digit at a time from those mod L_{d-1}
 * Range:     u8 n <= 16, u16 n <= 257, u32 n <= 1034, u64 n <= 1034,
 *            u128 n <= 1034, big any n
 * Helpers:   u128_to_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/u128_to_seq.h"

typedef unsigned __int128 u128;

static u128 addm(u128 a, u128 b, u128 M) { u128 s = a + b; return s >= M ? s - M : s; }
static u128 subm(u128 a, u128 b, u128 M) { return a >= b ? a - b : a + (M - b); }

static u128 mulm(u128 a, u128 b, u128 M) {
    if (M <= ((u128)1 << 64)) return a * b % M;
    u128 r = 0;
    int i;
    for (i = 127; i >= 0; i--) {
        r = addm(r, r, M);
        if ((b >> i) & 1) r = addm(r, a, M);
    }
    return r;
}

static u128 fibm(u128 k, u128 M) {
    u128 f0 = 0, f1 = 1 % M;
    int top = 127, i;
    while (top >= 0 && !((k >> top) & 1)) top--;
    for (i = top; i >= 0; i--) {
        u128 t = subm(addm(f1, f1, M), f0, M);
        u128 c = mulm(f0, t, M);
        u128 d = addm(mulm(f0, f0, M), mulm(f1, f1, M), M);
        if ((k >> i) & 1) { f0 = d; f1 = addm(c, d, M); }
        else { f0 = c; f1 = d; }
    }
    return f0;
}

static u128 *terms = 0;
static size_t nterms = 0, capterms = 0;
static u128 *res = 0;
static size_t nres = 0;
static int level = 0;
static u128 cur_mod = 0, cur_L = 0;

static int cmp128(const void *x, const void *y) {
    u128 a = *(const u128 *)x, b = *(const u128 *)y;
    return a < b ? -1 : (a > b ? 1 : 0);
}

static void push_term(u128 v) {
    if (nterms == capterms) {
        capterms = capterms ? capterms * 2 : 64;
        terms = (u128 *)realloc(terms, capterms * sizeof(u128));
    }
    terms[nterms++] = v;
}

static void emit_level(u128 lo, u128 hi) {
    size_t start = nterms, i;
    for (i = 0; i < nres; i++)
        if (res[i] >= lo && res[i] < hi) push_term(res[i]);
    qsort(terms + start, nterms - start, sizeof(u128), cmp128);
}

static void next_level(void) {
    if (level == 0) {
        u128 r;
        cur_mod = 10; cur_L = 60;
        res = (u128 *)malloc(60 * sizeof(u128));
        nres = 0;
        for (r = 0; r < 60; r++)
            if (fibm(r, 10) == r % 10) res[nres++] = r;
        level = 1;
        emit_level(0, 10);
        return;
    }
    {
        u128 nmod = cur_mod * 10;
        u128 nL = (level == 1) ? (u128)300 : cur_L * 10;
        unsigned factor = (unsigned)(nL / cur_L), j;
        size_t cap = nres * factor + 1, cnt = 0, i;
        u128 *nr = (u128 *)malloc(cap * sizeof(u128));
        for (i = 0; i < nres; i++)
            for (j = 0; j < factor; j++) {
                u128 x = res[i] + (u128)j * cur_L;
                if (fibm(x, nmod) == x % nmod) nr[cnt++] = x;
            }
        free(res);
        res = nr; nres = cnt;
        emit_level(cur_mod, nmod);
        cur_mod = nmod; cur_L = nL;
        level++;
    }
}

seq_t a(idx_t n) {
    while (nterms < n && level < 37) next_level();
    if (n == 0 || n > nterms) return 0;
    return u128_to_seq(terms[n - 1]);
}
