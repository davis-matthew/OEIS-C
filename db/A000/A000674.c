/*
 * A000674  Boustrophedon transform of 1, 2, 2, 2, 2, ...
 * https://oeis.org/A000674
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   Boustrophedon (Seidel) triangle of b = 1, 2, 2, 2, ...: T(k,0) =
 *            b(k), T(k,j) = T(k,j-1) + T(k-1,k-j) for 1 <= j <= k, a(n) =
 *            T(n,n), with b(0) = 1 and b(k) = 2 for k >= 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 14, u64 n <= 23, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static seq_t *row = NULL;   /* last computed row, length done */
static seq_t *nrow = NULL;  /* scratch row */
static seq_t *vals = NULL;  /* vals[k] = a(k) for k < done */
static idx_t done = 0;
static idx_t cap = 0;

static void ensure_cap(idx_t need) {
    if (need <= cap) return;
    idx_t nc = cap * 2;
    if (nc < need) nc = need;
    if (nc < 64) nc = 64;
    seq_t *r = (seq_t *)calloc((size_t)nc, sizeof(seq_t));
    seq_t *s = (seq_t *)calloc((size_t)nc, sizeof(seq_t));
    seq_t *v = (seq_t *)calloc((size_t)nc, sizeof(seq_t));
    for (idx_t i = 0; i < done; i++) {
        r[i] = row[i];
        v[i] = vals[i];
    }
    free(row);
    free(nrow);
    free(vals);
    row = r;
    nrow = s;
    vals = v;
    cap = nc;
}

seq_t a(idx_t n) {
    if (n < done) return vals[n];
    ensure_cap(n + 1);
    for (idx_t k = done; k <= n; k++) {
        nrow[0] = (k == 0) ? (seq_t)1 : (seq_t)2;
        for (idx_t j = 1; j <= k; j++) {
            nrow[j] = nrow[j - 1] + row[k - j];
        }
        vals[k] = nrow[k];
        seq_t *tmp = row;
        row = nrow;
        nrow = tmp;
        done = k + 1;
    }
    return vals[n];
}
