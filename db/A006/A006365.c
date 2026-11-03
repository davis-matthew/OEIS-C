/*
 * A006365  Number of binary tree partitions.
 * https://oeis.org/A006365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   G(z) = lim_{m->oo} G_m(z), where G_0 = 1 and G_m = G_{m-1} +
 *            z^m*G_m^2 (m >= 1), which is the same as G_m(z) = (2z^m)^(-1)*(1
 *            - sqrt(1 - 4z^m * Sum_{k=0..m-1} z^k*G_k(z)^2)). With g_m(j) =
 *            [z^j] G_m: g_0(j) = [j = 0], g_m(j) = g_{m-1}(j) +
 *            Sum_{i=0..j-m} g_m(i)*g_m(j-m-i) for 1 <= m <= j, g_m(j)...
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 19, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Validated: partial, pass, 31 terms through n = 30 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdbool.h>
#include <stdint.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static bool seq_is_fixed_width(void)
{
    seq_t t = 1;
    int i;
    for (i = 0; i < 256; i++) {
        t *= 2;
        if (t == 0)
            return true;
    }
    return false;
}

static seq_t **col = NULL;   /* col[j][m] = g_m(j) for m = 0..j */
static idx_t done = 0;       /* columns 0..done-1 are computed */
static idx_t cap = 0;

static seq_t gval(idx_t m, idx_t i)
{
    return col[i][m < i ? m : i];
}

seq_t a(idx_t n)
{
    idx_t j, m, i, t;
    /* a(n) >= Catalan(n), so beyond n = 200 no term fits in 128 bits */
    if (seq_is_fixed_width() && n > 200)
        return (seq_t)0;
    if (n >= cap) {
        idx_t nc = cap ? cap : 16;
        while (nc <= n) nc *= 2;
        col = (seq_t **)realloc(col, (size_t)nc * sizeof(seq_t *));
        for (j = cap; j < nc; j++) col[j] = NULL;
        cap = nc;
    }
    for (j = done; j <= n; j++) {
        seq_t *c = alloc_zero(j + 1);
        col[j] = c;
        c[0] = (j == 0) ? (seq_t)1 : (seq_t)0;
        for (m = 1; m <= j; m++) {
            seq_t s = 0;
            t = j - m;
            /* columns < j are complete; g_m(i) with i <= t < j is available */
            for (i = 0; 2 * i < t; i++)
                s += gval(m, i) * gval(m, t - i);
            s = s + s;
            if (t % 2 == 0) {
                seq_t h = gval(m, t / 2);
                s += h * h;
            }
            c[m] = c[m - 1] + s;
        }
        done = j + 1;
    }
    return col[n][n];
}
