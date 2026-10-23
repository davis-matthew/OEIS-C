/*
 * A157148  Triangle T(n, k, m) = (m*(n-k) + 1)*T(n-1, k-1, m) + (m*k +
 *          1)*T(n-1, k, m) + m*k*(n-k)*T(n-2, k-1, m) with T(n, 0, m) = T(n,
 *          n, m) = 1 and m = 2, read by rows.
 * https://oeis.org/A157148
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is read from triangle T(n,k,m) with m = 2 defined by
 *            T(n,0)=T(n,n)=1, and for 1 <= k <= n-1, T(n,k) =
 *            (2*(n-k)+1)*T(n-1,k-1) + (2*k+1)*T(n-1,k) +
 *            2*k*(n-k)*T(n-2,k-1). The sequence a(n) is obtained by reading
 *            the triangle row by row (offset 0)
 * Range:     u8 n <= 11, u16 n <= 29, u32 n <= 59, u64 n <= 159,
 *            u128 n <= 418, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

static inline size_t offset_row(size_t row)
{
    return row * (row + 1) / 2;
}

seq_t a(idx_t n)
{
    /* Find the row r containing index n. */
    size_t target = (size_t)n;
    size_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= target)
        ++r;

    /* Static storage for the triangle up to the largest row seen. */
    static seq_t *tri = NULL;
    static size_t tri_rows = 0;      /* number of rows already computed   */
    static size_t allocated_size = 0;/* total allocated cells */

    /* Extend the triangle if necessary. */
    if (r + 1 > tri_rows) {
        size_t needed_rows = r + 1;
        size_t new_size = needed_rows * (needed_rows + 1) / 2;

        /* Reallocate the array (preserves previous data). */
        seq_t *tmp = (seq_t *)realloc(tri, new_size * sizeof(seq_t));
        if (!tmp) {
            /* Allocation failure – return 0 as a safe fallback. */
            return 0;
        }
        tri = tmp;
        allocated_size = new_size;

        /* Compute rows that have not been computed yet. */
        for (size_t cur = tri_rows; cur <= r; ++cur) {
            size_t cur_off = offset_row(cur);
            tri[cur_off] = 1;                 /* left boundary */
            tri[cur_off + cur] = 1;           /* right boundary */

            if (cur >= 2) {
                size_t prev_off   = offset_row(cur - 1);
                size_t prevprev_off = offset_row(cur - 2);

                for (size_t k = 1; k < cur; ++k) {
                    seq_t term1 = ((seq_t)2 * (seq_t)(cur - k) + 1) *
                                  tri[prev_off + (k - 1)];
                    seq_t term2 = ((seq_t)2 * (seq_t)k + 1) *
                                  tri[prev_off + k];
                    seq_t term3 = ((seq_t)2 * (seq_t)k *
                                   (seq_t)(cur - k)) *
                                  tri[prevprev_off + (k - 1)];
                    tri[cur_off + k] = term1 + term2 + term3;
                }
            }
        }
        tri_rows = needed_rows;
    }

    size_t row_start = offset_row(r);
    size_t pos_in_row = target - row_start;
    return tri[row_start + pos_in_row];
}
