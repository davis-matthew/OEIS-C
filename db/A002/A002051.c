/*
 * A002051  Steffensen's bracket function [n,2].
 * https://oeis.org/A002051
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(1) = 0; a(n) = A000670(n-1) - 2^(n-2) for n >= 2, where
 *            A000670(m) = Sum_{k=0..m} Stirling2(m,k)*k!
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 31,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *st_row = NULL;   /* current Stirling2 row S(rows_done, k) */
static seq_t *fub = NULL;      /* fub[m] = A000670(m) */
static idx_t rows_done = 0;
static idx_t cap = 0;

static void ensure(idx_t m)
{
    idx_t i, j, k;
    if (st_row == NULL) {
        cap = (m + 1 > 16) ? m + 1 : 16;
        st_row = (seq_t *)malloc(cap * sizeof(seq_t));
        fub = (seq_t *)malloc(cap * sizeof(seq_t));
        for (i = 0; i < cap; i++) { st_row[i] = 0; fub[i] = 0; }
        st_row[0] = 1;
        fub[0] = 1;
        rows_done = 0;
    }
    if (m + 1 > cap) {
        idx_t nc = cap * 2;
        seq_t *ns, *nf;
        if (nc < m + 1) nc = m + 1;
        ns = (seq_t *)malloc(nc * sizeof(seq_t));
        nf = (seq_t *)malloc(nc * sizeof(seq_t));
        for (i = 0; i < nc; i++) {
            if (i < cap) { ns[i] = st_row[i]; nf[i] = fub[i]; }
            else { ns[i] = 0; nf[i] = 0; }
        }
        free(st_row);
        free(fub);
        st_row = ns;
        fub = nf;
        cap = nc;
    }
    for (i = rows_done + 1; i <= m; i++) {
        seq_t sum, fact;
        for (j = i; j >= 1; j--)
            st_row[j] = (seq_t)j * st_row[j] + st_row[j - 1];
        st_row[0] = 0;
        sum = 0;
        fact = 1;
        for (k = 1; k <= i; k++) {
            fact *= (seq_t)k;
            sum += st_row[k] * fact;
        }
        fub[i] = sum;
        rows_done = i;
    }
}

seq_t a(idx_t n)
{
    idx_t m, i;
    seq_t p2;
    if (n <= 1)
        return 0;
    m = n - 1;
    ensure(m);
    p2 = 1;
    for (i = 0; i < n - 2; i++)
        p2 *= 2;
    return fub[m] - p2;
}
