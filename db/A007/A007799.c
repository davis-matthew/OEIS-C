/*
 * A007799  Irregular triangle read by rows: Whitney numbers of the second
 *          kind a(n,k), n >= 1, k >= 0, for the star poset.
 * https://oeis.org/A007799
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabf, easy, nice
 * Formula:   T(n,0) = 1, T(n,1) = n-1, T(n,2) = (n-1)(n-2); T(n,k) = T(n-1,k)
 *            + (n-1)T(n-1,k-1) - (n-2)T(n-2,k-1) + (n-2)T(n-2,k-3) for k >=
 *            3, row n has floor(3(n-1)/2)+1 entries, k = 0..floor(3(n-1)/2);
 *            a(m) reads rows in order
 * Range:     u8 n <= 31, u16 n <= 55, u32 n <= 138, u64 n <= 350,
 *            u128 n <= 899, big any n
 * Helpers:   alloc_zero, row_len
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/alloc_zero.h"
#include "../utils/row_len.h"

static seq_t **rows = NULL;
static idx_t *lens = NULL;
static idx_t nrows = 0; /* rows 1..nrows are built; index 0 unused */
static idx_t cap = 0;

static seq_t get(idx_t r, idx_t k, long long kk) {
    if (r < 1 || kk < 0) return (seq_t)0;
    k = (idx_t)kk;
    if (k >= lens[r]) return (seq_t)0;
    return rows[r][k];
}

static void build_to(idx_t R) {
    if (R <= nrows) return;
    if (R + 1 > cap) {
        idx_t nc = (R + 1) * 2;
        rows = (seq_t **)realloc(rows, nc * sizeof(seq_t *));
        lens = (idx_t *)realloc(lens, nc * sizeof(idx_t));
        cap = nc;
    }
    for (idx_t r = nrows + 1; r <= R; r++) {
        idx_t L = row_len(r);
        seq_t *row = alloc_zero(L);
        for (idx_t k = 0; k < L; k++) {
            if (k == 0) row[k] = (seq_t)1;
            else if (k == 1) row[k] = (seq_t)(r - 1);
            else if (k == 2) row[k] = (seq_t)(r - 1) * (seq_t)(r - 2);
            else {
                long long kk = (long long)k;
                seq_t v = get(r - 1, 0, kk);
                v += (seq_t)(r - 1) * get(r - 1, 0, kk - 1);
                v += (seq_t)(r - 2) * get(r - 2, 0, kk - 3);
                v -= (seq_t)(r - 2) * get(r - 2, 0, kk - 1);
                row[k] = v;
            }
        }
        rows[r] = row;
        lens[r] = L;
        nrows = r;
    }
}

seq_t a(idx_t n) {
    idx_t r = 1, start = 0;
    while (n > start + row_len(r)) {
        start += row_len(r);
        r++;
    }
    idx_t col = n - start - 1;
    build_to(r);
    return rows[r][col];
}
