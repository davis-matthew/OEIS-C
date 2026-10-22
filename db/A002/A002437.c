/*
 * A002437  a(n) = A000364(n) * (3^(2*n+1) + 1)/4.
 * https://oeis.org/A002437
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A000364(n) * (3^(2*n+1) + 1)/4, where A000364(n) =
 *            A000111(2n) comes from the Seidel-Entringer triangle E(0,0)=1,
 *            E(k,0)=0, E(k,j)=E(k,j-1)+E(k-1,k-j), A000111(k)=E(k,k); and
 *            c(n) = (3^(2n+1)+1)/4 satisfies c(0)=1, c(n)=9*c(n-1)-2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 14,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>

static seq_t *zig = NULL;
static idx_t zcap = 0;

static void build_zig(idx_t m) {
    idx_t cap = m + 1;
    idx_t grow = zcap + zcap / 2;
    idx_t k, j;
    seq_t *z, *prev, *cur, *tmp;
    if (cap < grow) cap = grow;
    if (cap < 16) cap = 16;
    z = (seq_t *)calloc((size_t)cap, sizeof(seq_t));
    prev = (seq_t *)calloc((size_t)cap, sizeof(seq_t));
    cur = (seq_t *)calloc((size_t)cap, sizeof(seq_t));
    prev[0] = 1;
    z[0] = 1;
    for (k = 1; k < cap; k++) {
        cur[0] = 0;
        for (j = 1; j <= k; j++)
            cur[j] = cur[j - 1] + prev[k - j];
        z[k] = cur[k];
        tmp = prev; prev = cur; cur = tmp;
    }
    free(prev);
    free(cur);
    if (zig != NULL) free(zig);
    zig = z;
    zcap = cap;
}

seq_t a(idx_t n) {
    idx_t m = 2 * n;
    idx_t i;
    seq_t c = 1;
    if (m >= zcap) build_zig(m);
    for (i = 0; i < n; i++)
        c = c * 9 - 2;
    return zig[m] * c;
}
