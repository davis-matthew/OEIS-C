/*
 * A136745  Irregular triangle T(0,0)=1, T(1,0)=-1, T(1,1)=0, T(1,2)=1 and
 *          T(n,k) = T(n-1,k-2)-T(n-2,k).
 * https://oeis.org/A136745
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, tabf, sign
 * Formula:   * a(n) = T(r,c), where r = ⌊√n⌋, c = n - r², * T satisfies:
 *            T(0,0)=1, * T(1,0)=-1, T(1,1)=0, T(1,2)=1, *
 *            T(n,k)=T(n-1,k-2)-T(n-2,k) for n≥2, * out‑of‑range entries are
 *            treated as 0. * The sequence lists the triangle entries in
 *            row‑major order
 * Range:     i8 n <= 107, i16 n <= 107, i32 n <= 107, i64 n <= 107,
 *            i128 n <= 107, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

/* Integer square root: floor(sqrt(n)) for 0 ≤ n < 2⁶⁴. */
static inline idx_t floor_isqrt(idx_t n) {
    idx_t low = 0;
    idx_t high = (n < 0xFFFFFFFFULL) ? n : 0xFFFFFFFFULL;
    while (low <= high) {
        idx_t mid = low + ((high - low) >> 1);
        idx_t sq = mid * mid;
        if (sq == n) return mid;
        if (sq < n) low = mid + 1;
        else high = (mid != 0) ? mid - 1 : mid;
    }
    return high;
}

seq_t a(idx_t n) {
    /* Static cache of triangle rows. */
    static seq_t **rows = NULL;
    static idx_t rows_cap = 0;
    static idx_t rows_cnt = 0;

    idx_t r = floor_isqrt(n);              /* required row index         */

    /* Grow the rows array if necessary. */
    if (r >= rows_cap) {
        idx_t new_cap = rows_cap ? rows_cap << 1 : 1;
        while (new_cap <= r) new_cap <<= 1;
        seq_t **new_rows = (seq_t**)realloc(rows, new_cap * sizeof(seq_t*));
        if (!new_rows) return 0;           /* out‑of‑memory; caller handles */
        rows = new_rows;
        for (idx_t i = rows_cap; i < new_cap; ++i) rows[i] = NULL;
        rows_cap = new_cap;
    }

    /* Compute any missing rows up to r. */
    for (idx_t i = rows_cnt; i <= r; ++i) {
        idx_t len = 2 * i + 1;
        seq_t *row = (seq_t*)malloc(len * sizeof(seq_t));
        if (!row) return 0;                /* out‑of‑memory; caller handles */

        if (i == 0) {
            row[0] = 1;
        } else if (i == 1) {
            row[0] = -1;
            row[1] = 0;
            row[2] = 1;
        } else {
            idx_t len_prevprev = 2 * (i - 2) + 1;
            for (idx_t k = 0; k < len; ++k) {
                seq_t val = 0;
                if (k >= 2) val += rows[i - 1][k - 2];
                if (k < len_prevprev) val -= rows[i - 2][k];
                row[k] = val;
            }
        }
        rows[i] = row;
    }
    rows_cnt = r + 1;

    /* Retrieve the desired triangle entry. */
    idx_t start = r * r;
    idx_t col   = n - start;
    return rows[r][col];
}
