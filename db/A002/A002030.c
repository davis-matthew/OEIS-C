/*
 * A002030  Number of connected graphs on n labeled nodes, each node being
 *          colored with one of 5 colors, such that no edge joins nodes of the
 *          same color.
 * https://oeis.org/A002030
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   c_1(m) = 1, c_k(m) = Sum_{j=0..m}
 *            C(m,j)*2^(j*(m-j))*c_{k-1}(m-j) (5-colored graphs); a(0) = 1,
 *            a(m) = c_5(m) - Sum_{k=1..m-1} C(m-1,k-1)*a(k)*c_5(m-k) (E.g.f.:
 *            log of colored graphs)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 15,
 *            big any n
 * Validated: standard, pass, 31 terms through n = 49 (2026-11-03)
 * Helpers:   mem, pow2_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/pow2_u64.h"
#include "../utils/mem.h"

static seq_t *a2030_cache = NULL;
static idx_t a2030_len = 0;

static void a2030_build(idx_t n) {
    idx_t L = n + 1;
    seq_t *row = mem_new(seq_t, L);
    seq_t *prev = mem_new(seq_t, L);
    seq_t *pw = mem_new(seq_t, L);
    seq_t *av = mem_new(seq_t, L);
    seq_t *c[6];
    int k;
    idx_t m, j, t;
    for (k = 1; k <= 5; k++) c[k] = mem_new(seq_t, L);

    for (m = 0; m <= n; m++) {
        /* prev = C(m-1, .), row becomes C(m, .) */
        for (j = 0; j <= m; j++) prev[j] = row[j];
        row[m] = 1;
        for (t = m; t > 1; t--) {
            idx_t i = t - 1;
            row[i] += row[i - 1];
        }
        row[0] = 1;

        for (j = 0; j <= m; j++) {
            if (j <= m - j) pw[j] = pow2_u64((uint64_t)j * (uint64_t)(m - j));
            else pw[j] = pw[m - j];
        }

        c[1][m] = 1;
        for (k = 2; k <= 5; k++) {
            seq_t s = 0;
            for (j = 0; j <= m; j++) s += row[j] * pw[j] * c[k - 1][m - j];
            c[k][m] = s;
        }

        if (m == 0) {
            av[0] = 1;
        } else {
            seq_t s = c[5][m];
            for (j = 1; j < m; j++) s -= prev[j - 1] * av[j] * c[5][m - j];
            av[m] = s;
        }
    }

    mem_free(row);
    mem_free(prev);
    mem_free(pw);
    for (k = 1; k <= 5; k++) mem_free(c[k]);
    if (a2030_cache) mem_free(a2030_cache);
    a2030_cache = av;
    a2030_len = L;
}

seq_t a(idx_t n) {
    if (n >= a2030_len) {
        idx_t target = n;
        if (a2030_len > 0 && target < 2 * a2030_len) target = 2 * a2030_len;
        if (target < 16) target = 16;
        a2030_build(target);
    }
    return a2030_cache[n];
}
