/*
 * A072091  Permutation of natural numbers induced by the automorphism
 *          car_robl_cdr! acting on the parenthesizations encoded by A014486.
 * https://oeis.org/A072091
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A014486-rank of f(t), where t is the binary tree with
 *            Dyck word A014486(n) (word "1 A 0 B" encodes the tree (A . B),
 *            empty = nil) and f(nil) = nil, f((a . nil)) = (nil . a), f((a .
 *            (b . c))) = ((f(a) . b) . f(c)). n is unranked and the result
 *            ranked by counting Dyck paths, using a Catalan-...
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Validated: partial, pass, 70 terms through n = 69 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include "../utils/mem.h"

/* find index of the 0 matching the 1 at position s */
static int a072091_match(const char *w, int s)
{
    int d = 0, i = s;
    for (;; i++) {
        d += (w[i] == '1') ? 1 : -1;
        if (d == 0) return i;
    }
}

/* apply f to the tree encoded by w[s..e), append encoding to out */
static void a072091_f(const char *w, int s, int e, char *out, int *o)
{
    if (s >= e) return;
    int i = a072091_match(w, s);
    int a_s = s + 1, a_e = i;
    int bs = i + 1;
    if (bs >= e) {
        /* (a . nil) -> (nil . a) */
        out[(*o)++] = '1'; out[(*o)++] = '0';
        for (int k = a_s; k < a_e; k++) out[(*o)++] = w[k];
        return;
    }
    int j = a072091_match(w, bs);
    int b_s = bs + 1, b_e = j;
    int c_s = j + 1, c_e = e;
    /* ((f(a) . b) . f(c)) */
    out[(*o)++] = '1';
    out[(*o)++] = '1';
    a072091_f(w, a_s, a_e, out, o);
    out[(*o)++] = '0';
    for (int k = b_s; k < b_e; k++) out[(*o)++] = w[k];
    out[(*o)++] = '0';
    a072091_f(w, c_s, c_e, out, o);
}

seq_t a(idx_t n)
{
    seq_t r = (seq_t)n;
    int m = 0;
    seq_t C = 1;
    while (r >= C) {
        r -= C;
        C = C * (seq_t)(2 * (2 * m + 1)) / (seq_t)(m + 2);
        m++;
    }
    if (m == 0) return (seq_t)0;
    seq_t base = (seq_t)n - r;
    int L = 2 * m;
    int cols = L + 2;
    seq_t *W = mem_new(seq_t, (size_t)(L + 1) * (size_t)cols);
    W[0] = 1;
    for (int rem = 1; rem <= L; rem++) {
        for (int h = 0; h <= L; h++) {
            seq_t v = W[(rem - 1) * cols + h + 1];
            if (h > 0) v += W[(rem - 1) * cols + h - 1];
            W[rem * cols + h] = v;
        }
    }
    char *w = (char *)malloc((size_t)L + 2);
    char *o = (char *)malloc((size_t)L + 2);
    /* unrank */
    int h = 0;
    for (int pos = 0; pos < L; pos++) {
        int rem = L - pos;
        if (h > 0) {
            seq_t c0 = W[(rem - 1) * cols + (h - 1)];
            if (r < c0) { w[pos] = '0'; h--; }
            else { r -= c0; w[pos] = '1'; h++; }
        } else {
            w[pos] = '1'; h++;
        }
    }
    int ol = 0;
    a072091_f(w, 0, L, o, &ol);
    /* rank */
    seq_t rk = 0;
    h = 0;
    for (int pos = 0; pos < ol; pos++) {
        if (o[pos] == '1') {
            if (h > 0) rk += W[(L - pos - 1) * cols + (h - 1)];
            h++;
        } else {
            h--;
        }
    }
    free(w);
    free(o);
    mem_free(W);
    return base + rk;
}
