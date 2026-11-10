/*
 * A201093  Triangle T(n,k), read by rows, given by
 *          (0,1,-1,-1,1,1,-1,-1,1,1,-1,-1,1,...) DELTA
 *          (1,0,0,0,0,0,0,0,0,0,0,...) where DELTA is the operator defined in
 *          A084938.
 * https://oeis.org/A201093
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   Riordan array (1, x*f(x)), f = g.f. of A097331 (f = 1 +
 *            x*C(x^2), C = Catalan g.f.); T(n,k) = [x^n] (x*f(x))^k for k>=1,
 *            T(n,0) = 0^n; a(n) is the triangle read by rows
 * Range:     u8 n <= 65, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Validated: partial, pass, 66 terms through n = 65 (2026-11-07)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/mem.h"

seq_t a(idx_t n)
{
    idx_t r = 0, cum = 0;
    while (cum + r + 1 <= n) { cum += r + 1; r++; }
    idx_t k = n - cum;

    if (k == 0) return (seq_t)(r == 0 ? 1 : 0);
    if (r == 0) return (seq_t)1;

    size_t R = (size_t)r;
    size_t nc = R / 2 + 2;
    seq_t *c = mem_new(seq_t, nc);
    seq_t *g = mem_new(seq_t, R + 1);
    seq_t *p = mem_new(seq_t, R + 1);
    seq_t *q = mem_new(seq_t, R + 1);

    /* Catalan numbers by convolution (no division) */
    c[0] = 1;
    for (size_t j = 1; j < nc; j++) {
        seq_t s = 0;
        for (size_t i = 0; i < j; i++) s += c[i] * c[j - 1 - i];
        c[j] = s;
    }

    /* g = x*f(x) */
    g[1] = 1;
    for (size_t m = 2; m <= R; m++)
        g[m] = (m % 2 == 0) ? c[(m - 2) / 2] : (seq_t)0;

    for (size_t m = 0; m <= R; m++) p[m] = g[m];
    for (idx_t e = 1; e < k; e++) {
        for (size_t m = 0; m <= R; m++) {
            seq_t s = 0;
            for (size_t i = 1; i + 1 <= m; i++) s += g[i] * p[m - i];
            q[m] = s;
        }
        for (size_t m = 0; m <= R; m++) p[m] = q[m];
    }

    seq_t res = p[R];
    mem_free(c); mem_free(g); mem_free(p); mem_free(q);
    return res;
}
