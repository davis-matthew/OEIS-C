/*
 * A007046  Number of irreducible positions of size n in Montreal solitaire.
 * https://oeis.org/A007046
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = d(n, 2) where d(n, k) = 0 if n < k*(k+1)/2, d(n, k) = 1
 *            if n = k*(k+1)/2, and d(n, k) = d(n, k+1) + Sum_{r=1..k}
 *            binomial(k+1, r) * d(n - k*(k+1)/2 + r*(r-1)/2, r) if n >
 *            k*(k+1)/2 [Cannings and Haigh]. Tabulated by increasing n, k
 *            descending
 * Range:     u8 n <= 8, u16 n <= 13, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   alloc_zero, tri
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/tri.h"

static seq_t **rows = NULL;   /* rows[m][k] = d(m, k), k = 1..rowlen[m] */
static idx_t *rowlen = NULL;
static idx_t nrows = 0;

static seq_t **pas = NULL;    /* pas[i][j] = binomial(i, j) */
static idx_t npas = 0;

static void ensure_pascal(idx_t upto) {
    if (upto < npas) return;
    idx_t nn = upto + 1;
    pas = (seq_t **)realloc(pas, nn * sizeof(seq_t *));
    for (idx_t i = npas; i < nn; i++) {
        pas[i] = alloc_zero(i + 1);
        pas[i][0] = 1;
        pas[i][i] = 1;
        for (idx_t j = 1; j < i; j++) pas[i][j] = pas[i - 1][j - 1] + pas[i - 1][j];
    }
    npas = nn;
}

static seq_t getd(idx_t m, idx_t k) {
    if (k > rowlen[m]) return 0;
    return rows[m][k];
}

static void ensure_rows(idx_t n) {
    if (n < nrows) return;
    idx_t nn = n + 1;
    rows = (seq_t **)realloc(rows, nn * sizeof(seq_t *));
    rowlen = (idx_t *)realloc(rowlen, nn * sizeof(idx_t));
    for (idx_t m = nrows; m < nn; m++) {
        idx_t K = 0;
        while (tri(K + 1) <= m) K++;
        ensure_pascal(K + 1);
        rows[m] = alloc_zero(K + 2);
        rowlen[m] = K;
        for (idx_t k = K; k >= 1; k--) {
            idx_t tk = tri(k);
            if (tk == m) {
                rows[m][k] = 1;
                continue;
            }
            seq_t v = 0;
            if (k < K) v = rows[m][k + 1];
            for (idx_t r = 1; r <= k; r++) {
                idx_t x = m - tk + (r * (r - 1) / 2);
                if (r > rowlen[x]) continue;
                v += pas[k + 1][r] * rows[x][r];
            }
            rows[m][k] = v;
        }
    }
    nrows = nn;
}

seq_t a(idx_t n) {
    ensure_rows(n);
    return getd(n, 2);
}
