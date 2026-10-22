/*
 * A001180  Erroneous version of A002572.
 * https://oeis.org/A001180
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = A002572(n) = f(n,1) for n >= 5, where f(0,0) = 1, f(m,0)
 *            = 0 for m > 0, f(m,k) = 0 for k > m, and f(m,k) = f(m-1,k-1) +
 *            f(m,2k) for k >= 1. The erroneous OEIS terms a(1..4) = 1, 1, 2,
 *            3 are given explicitly
 * Range:     i8 n <= 11, i16 n <= 20, i32 n <= 20, i64 n <= 20, i128 n <= 20,
 *            big any n
 * Helpers:   a306_grow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/a306_grow.h"

static seq_t *res_tab = 0;   /* res_tab[m] = f(m,1) */
static seq_t *row_prev = 0;  /* row m-1 of f */
static seq_t *row_cur = 0;
static idx_t done_m = 0;
static idx_t cap = 0;

static void ensure(idx_t n)
{
    idx_t m, k;
    if (cap == 0) {
        idx_t c = 64;
        while (c < n + 1) c *= 2;
        res_tab = a306_grow(0, 0, c);
        row_prev = a306_grow(0, 0, c);
        row_cur = a306_grow(0, 0, c);
        cap = c;
        row_prev[0] = 1; /* f(0,0) = 1 */
        done_m = 0;
    }
    if (n <= done_m) return;
    if (n + 1 > cap) {
        idx_t c = cap;
        while (c < n + 1) c *= 2;
        res_tab = a306_grow(res_tab, cap, c);
        row_prev = a306_grow(row_prev, cap, c);
        row_cur = a306_grow(row_cur, cap, c);
        cap = c;
    }
    for (m = done_m + 1; m <= n; m++) {
        seq_t *t;
        for (k = m; k >= 1; k--) {
            seq_t v = row_prev[k - 1];
            if (2 * k <= m) v += row_cur[2 * k];
            row_cur[k] = v;
        }
        row_cur[0] = 0;
        res_tab[m] = row_cur[1];
        t = row_prev; row_prev = row_cur; row_cur = t;
    }
    done_m = n;
}

seq_t a(idx_t n)
{
    if (n <= 2) return 1;
    if (n == 3) return 2;
    if (n == 4) return 3;
    ensure(n);
    return res_tab[n];
}
