/*
 * A000697  Boustrophedon transform of 1, 1, 4, 9, 16, ...
 * https://oeis.org/A000697
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon transform of b = 1, 1, 4, 9, 16, ... (b(0) = 1,
 *            b(i) = i^2): T(0,0) = 1, T(i,0) = b(i), T(i,k) = T(i,k-1) +
 *            T(i-1,i-k), a(n) = T(n,n)
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 14, u64 n <= 22, u128 n <= 37,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 370 (2026-11-03)
 * Helpers:   mem
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/mem.h"

static seq_t *alloc_array(idx_t n) {
    seq_t *p = mem_new(seq_t, (n + 1));
    for (idx_t i = 0; i <= n; i++) {
        p[i] = 0;
    }
    return p;
}

static seq_t *memo = NULL;
static seq_t *rowA = NULL;
static seq_t *rowB = NULL;
static idx_t have = 0;
static idx_t cap = 0;

seq_t a(idx_t n) {
    idx_t i, k;
    if (n < have) return memo[n];

    if (n + 1 > cap) {
        idx_t nc = n + 1;
        if (nc < 2 * cap) nc = 2 * cap;
        seq_t *m = alloc_array(nc);
        seq_t *ra = alloc_array(nc);
        seq_t *rb = alloc_array(nc);
        for (i = 0; i < have; i++) {
            m[i] = memo[i];
            ra[i] = rowA[i];
        }
        if (memo) mem_free(memo);
        if (rowA) mem_free(rowA);
        if (rowB) mem_free(rowB);
        memo = m;
        rowA = ra;
        rowB = rb;
        cap = nc;
    }

    for (i = have; i <= n; i++) {
        if (i == 0) {
            rowA[0] = 1;
        } else {
            rowB[0] = (seq_t)i * (seq_t)i;
            for (k = 1; k <= i; k++)
                rowB[k] = rowB[k - 1] + rowA[i - k];
            seq_t *t = rowA;
            rowA = rowB;
            rowB = t;
        }
        memo[i] = rowA[i];
    }
    have = n + 1;
    return memo[n];
}
