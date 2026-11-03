/*
 * A005573  Number of walks on cubic lattice (starting from origin and not
 *          going below xy plane).
 * https://oeis.org/A005573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy, nice
 * Formula:   (n+1)*a(n) = 2*(4*n+1)*a(n-1) - 12*(n-1)*a(n-2), a(0) = 1, a(1)
 *            = 5; for fixed-width seq_t the walk count is computed directly
 *            by a height DP (f'(h) = 4*f(h) + f(h-1) + f(h+1), h >= 0, a(n) =
 *            Sum_h f(h)), which only uses + and * so it stays exact modulo
 *            the type width
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 12, u64 n <= 25, u128 n <= 50,
 *            big any n
 * Validated: standard, pass, 37 terms through n = 955 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/mem.h"

static int seq_is_big(void)
{
    seq_t x = 1;
    int i;
    for (i = 0; i < 130; i++) x = x * 2;
    return !(x == 0);
}

static seq_t a_dp(idx_t n) {
    seq_t *f = mem_new(seq_t, (size_t)(n + 2));
    seq_t *g = mem_new(seq_t, (size_t)(n + 2));
    idx_t h, s;
    seq_t total = 0;
    for (h = 0; h < n + 2; h++) { f[h] = 0; g[h] = 0; }
    f[0] = 1;
    for (s = 0; s < n; s++) {
        seq_t *t;
        for (h = 0; h <= s + 1; h++) {
            seq_t v = f[h] * 4 + f[h + 1];
            if (h > 0) v += f[h - 1];
            g[h] = v;
        }
        t = f; f = g; g = t;
    }
    for (h = 0; h <= n; h++) total += f[h];
    mem_free(f);
    mem_free(g);
    return total;
}

static seq_t a_rec(idx_t n) {
    seq_t p2 = 1, p1 = 5, cur;
    idx_t k;
    if (n == 0) return p2;
    if (n == 1) return p1;
    for (k = 2; k <= n; k++) {
        cur = ((seq_t)2 * ((seq_t)4 * (seq_t)k + 1) * p1
               - (seq_t)12 * ((seq_t)k - 1) * p2) / ((seq_t)k + 1);
        p2 = p1;
        p1 = cur;
    }
    return p1;
}

seq_t a(idx_t n) {
    if (seq_is_big()) return a_rec(n);
    return a_dp(n);
}
