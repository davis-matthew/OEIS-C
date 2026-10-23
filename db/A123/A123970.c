/*
 * A123970  Triangle read by rows: T(0,0)=1; T(n,k) is the coefficient of
 *          x^(n-k) in the monic characteristic polynomial of the n X n matrix
 *          (min(i,j)) (i,j=1,2,...,n) (0 <= k <= n, n >= 1).
 * https://oeis.org/A123970
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl
 * Formula:   not stated
 * Range:     i8 n <= 30, i16 n <= 71, i32 n <= 71, i64 n <= 71, i128 n <= 71,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Matrix–convolution triangle (OEIS A123970)
 * Formula:  T(0,0)=1;   T(1,0)=1, T(1,1)=-1;
 *            T(n,k)=2*T(n‑1,k)-T(n‑1,k‑1)-T(n‑2,k),   0≤k≤n;
 *             T(n,k)=0,  k<0 or k>n.
 * a(n) is the row‑by‑row reading of this triangle.
 */

#include <stdlib.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* Find row r and column k corresponding to index n. */
    idx_t low = 0, high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 <= n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 t = (unsigned __int128)mid * (mid + 1) / 2;
        if (t <= n) low = mid + 1;
        else        high = mid;
    }
    idx_t r = low - 1;
    idx_t k = n - (idx_t)((unsigned __int128)r * (r + 1) / 2);

    /* Static cache of the rows that have been built. */
    static seq_t **tri = NULL;
    static size_t tri_cap = 0;
    static idx_t max_row = (idx_t)-1;   /* no rows cached yet */

    /* Build rows up to r if we do not yet have them. */
    if (max_row == (idx_t)-1 || r > max_row) {
        size_t need_rows = (size_t)(r + 1);

        if (need_rows > tri_cap) {
            size_t new_cap = (tri_cap == 0) ? 1 : tri_cap;
            while (new_cap < need_rows) new_cap <<= 1;
            seq_t **tmp = (seq_t**)realloc(tri, new_cap * sizeof *tri);
            if (!tmp) exit(1);           /* allocation failed */
            tri = tmp;
            tri_cap = new_cap;
        }

        idx_t start = (max_row == (idx_t)-1) ? 0 : max_row + 1;
        for (idx_t rr = start; rr <= r; ++rr) {
            seq_t *row = (seq_t*)calloc((size_t)rr + 1, sizeof *row);
            if (!row) exit(1);           /* allocation failed */

            for (idx_t kk = 0; kk <= rr; ++kk) {
                seq_t val;
                if (rr == 0) {
                    val = 1;
                } else if (rr == 1) {
                    val = (kk == 0) ? 1 : -1;
                } else {
                    seq_t a1 = (kk <= rr - 1) ? tri[rr - 1][kk] : 0;
                    seq_t b1 = (kk >= 1)     ? tri[rr - 1][kk - 1] : 0;
                    seq_t c1 = (kk <= rr - 2) ? tri[rr - 2][kk] : 0;
                    val = 2 * a1 - b1 - c1;
                }
                row[kk] = val;
            }
            tri[rr] = row;
        }
        max_row = r;
    }

    return tri[r][k];
}
