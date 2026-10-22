/*
 * A002573  Restricted partitions.
 * https://oeis.org/A002573
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(1) = 0; a(n) = f(n-2, 4) for n >= 2. Here f(r, m) counts
 *            compositions of r whose first part is <= m and in which each
 *            later part is at most twice the part before it (this ordering
 *            reproduces the listed terms). f(0, m) = 1; f(r, 0) = 0 for r >
 *            0; f(r, m) = f(r, r) for m > r; f(r, m) = f(r, m-1)...
 * Range:     u8 n <= 12, u16 n <= 21, u32 n <= 40, u64 n <= 78, u128 n <= 154,
 *            big any n
 * Helpers:   zalloc_arr
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/zalloc_arr.h"

static seq_t **rows = NULL;
static idx_t built = 0;      /* rows 0 .. built-1 are computed */
static idx_t rows_cap = 0;

static seq_t F(idx_t r, idx_t m) {
    if (m > r) m = r;
    return rows[r][m];
}

static void build_to(idx_t R) {
    if (R + 1 <= built) return;
    if (R + 1 > rows_cap) {
        idx_t nc = rows_cap ? rows_cap : 16;
        while (nc < R + 1) nc *= 2;
        seq_t **nr = (seq_t **)realloc(rows, (size_t)nc * sizeof(seq_t *));
        rows = nr;
        rows_cap = nc;
    }
    for (idx_t r = built; r <= R; r++) {
        seq_t *row = zalloc_arr(r + 1);
        row[0] = (r == 0) ? (seq_t)1 : (seq_t)0;
        for (idx_t m = 1; m <= r; m++) {
            row[m] = row[m - 1] + F(r - m, 2 * m);
        }
        rows[r] = row;
        built = r + 1;
    }
}

seq_t a(idx_t n) {
    if (n < 2) return 0;
    build_to(n - 2);
    return F(n - 2, 4);
}
