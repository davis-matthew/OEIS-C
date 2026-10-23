/*
 * A142075  Triangle T(n, k) = 2^(k-1) * E(n, k-1) where E(n,k) are the
 *          Eulerian numbers A173018, read by rows.
 * https://oeis.org/A142075
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 12, u16 n <= 31, u32 n <= 71, u64 n <= 180,
 *            u128 n <= 510, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence:
   T(1,1)=1,
   T(n,1)=1,
   T(n,k)=2*(n-k+1)*T(n-1,k-1)+k*T(n-1,k)   (2 <= k <= n).
   Sequence a(n) is the triangle read by rows: a(n) = T(r,c)
   where r is the smallest integer with r*(r+1)/2 >= n.
   Initial terms: a(1)=1, a(2)=1, a(3)=2, a(4)=1, a(5)=8, ... */
#include <stddef.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    /* static cache of triangle entries */
    static seq_t *T      = NULL;
    static size_t  cap   = 0;
    static idx_t  last_r = 0;          /* highest row already computed */

    if (n < 1) return 0;              /* offset starts at 1 */

    /* Find the row r such that cumulative count r*(r+1)/2 >= n */
    idx_t low = 1, high = n, r = n;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        idx_t cum = (idx_t)((unsigned __int128)mid * (mid + 1) / 2);
        if (cum >= n)
            high = mid;
        else
            low = mid + 1;
    }
    r = low;

    /* offset of first element in this row (0‑based) */
    idx_t offset_before = (idx_t)((unsigned __int128)r * (r - 1) / 2);
    idx_t k = n - offset_before;          /* 1‑based column index */

    /* Grow cache if necessary */
    size_t new_cap = (size_t)((unsigned __int128)r * (r + 1) / 2);
    if (new_cap > cap) {
        seq_t *tmp = (seq_t *)realloc(T, new_cap * sizeof(seq_t));
        if (!tmp) return 0;               /* allocation failure */
        T   = tmp;
        cap = new_cap;
    }

    /* Compute new rows up to r */
    for (idx_t row = last_r + 1; row <= r; ++row) {
        idx_t row_off = (idx_t)((unsigned __int128)row * (row - 1) / 2);
        T[row_off] = 1;                    /* T(row,1) = 1 */

        idx_t prev_off = (idx_t)((unsigned __int128)(row - 1) * (row - 2) / 2);
        for (idx_t col = 2; col <= row; ++col) {
            seq_t t_prev_k_minus1 = T[prev_off + (col - 2)];
            seq_t t_prev_k = 0;
            if (col <= row - 1)
                t_prev_k = T[prev_off + (col - 1)];

            seq_t part1 = (seq_t)2 * (seq_t)(row - col + 1) * t_prev_k_minus1;
            seq_t part2 = (seq_t)col * t_prev_k;
            T[row_off + (col - 1)] = part1 + part2;
        }
    }
    last_r = r;

    return T[offset_before + (k - 1)];
}
