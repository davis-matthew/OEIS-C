/*
 * A006947  Two-rowed truncated monotone triangles.
 * https://oeis.org/A006947
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{x<y} f_{n-1}(x,y)*(y-x+1), where f_m counts the
 *            bottom m+1 rows of the first two diagonals of a monotone
 *            triangle: f_0(1,2) = 1 and f_m(x,y) = Sum_{x'<=x<=y'<=y}
 *            f_{m-1}(x',y') for 1<=x<y<=m+2 (computed level by level with
 *            prefix sums)
 * Range:     u8 n <= 4, u16 n <= 6, u32 n <= 11, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Validated: partial, pass, 20 terms through n = 20 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include "../utils/mem.h"

static seq_t *alloc_zero(idx_t len) {
    seq_t *p = mem_new(seq_t, (size_t)len);
    for (idx_t i = 0; i < len; i++) p[i] = 0;
    return p;
}

static void release(seq_t *v, idx_t len) {
    for (idx_t i = 0; i < len; i++) v[i] = 0;
    mem_free(v);
}

static seq_t *grow_zero(seq_t *old, idx_t oldlen, idx_t newlen) {
    seq_t *p = mem_new(seq_t, (size_t)newlen);
    if (p == NULL) return NULL;
    mem_set(p, 0, (size_t)newlen * sizeof(seq_t));
    if (old != NULL) {
        mem_copy(p, old, (size_t)oldlen * sizeof(seq_t));
        mem_free(old);
    }
    return p;
}

static seq_t *lev = NULL;   /* current level f_{lev_m}, stride lev_m+3 */
static idx_t lev_m = 0;
static seq_t *res = NULL;   /* res[k] = a(k+1) */
static idx_t res_len = 0;
static idx_t res_cap = 0;

seq_t a(idx_t n)
{
    if (lev == NULL) {
        lev = alloc_zero(9);
        lev[1 * 3 + 2] = 1;
        lev_m = 0;
        res_cap = 16;
        res = alloc_zero(res_cap);
        res[0] = 2;
        res_len = 1;
    }
    while (res_len < n) {
        idx_t m = lev_m + 1;
        idx_t so = m + 2, sn = m + 3;
        seq_t *g = lev;
        seq_t *f = alloc_zero(sn * sn);
        idx_t x, y;
        /* column prefix sums over x' */
        for (y = 0; y <= m + 1; y++)
            for (x = 1; x <= m + 1; x++)
                g[x * so + y] += g[(x - 1) * so + y];
        for (x = 1; x <= m + 1; x++) {
            seq_t run = 0;
            for (y = x; y <= m + 2; y++) {
                if (y <= m + 1) run += g[x * so + y];
                if (y > x) f[x * sn + y] = run;
            }
        }
        release(g, so * so);
        {
            seq_t s = 0;
            for (x = 1; x <= m + 1; x++)
                for (y = x + 1; y <= m + 2; y++)
                    s += f[x * sn + y] * (seq_t)(y - x + 1);
            lev = f;
            lev_m = m;
            if (res_len >= res_cap) {
                idx_t nc = res_cap * 2;
                res = grow_zero(res, res_cap, nc);
                res_cap = nc;
            }
            res[res_len++] = s;
        }
    }
    return res[n - 1];
}
