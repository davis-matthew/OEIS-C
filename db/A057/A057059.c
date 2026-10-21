/*
 * A057059  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...
 *          Define i(m) and j(m) by R(i(m),j(m)) = m. Then a(n) =
 *          j(A057027(n)).
 * https://oeis.org/A057059
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   let t = max k with k(k+1)/2 < n. i = n - t(t+1)/2 (row index
 *            inside the current triangle, 1‑based) j = (t²+3t+4)/2 - n
 *            (column index inside the current triangle, 1‑based) a(n) = i is
 *            even ? i/2 : j + (i-1)/2 (This equals j(A057027(n)) as defined
 *            in the OEIS entry.)
 * Range:     u8 n <= 85, u16 n <= 85, u32 n <= 85, u64 n <= 85, u128 n <= 85,
 *            big any n
 * Helpers:   triangular_floor_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include "../utils/triangular_floor_idx.h"

seq_t a(idx_t n)
{
    /* Find t, i, j according to the formula above */
    idx_t t = triangular_floor_idx(n);
    unsigned __int128 tri_t = (unsigned __int128)t * (t + 1) / 2;
    idx_t i = n - (idx_t)tri_t;

    unsigned __int128 tmp = ((unsigned __int128)t * t
                            + 3u * (unsigned __int128)t
                            + 4u) / 2;
    idx_t j = (idx_t)(tmp - n);

    /* Compute a(n) from i and j */
    if ((i & 1) != 0)   /* i odd */
        return (seq_t)(j + (i - 1) / 2);
    else                /* i even */
        return (seq_t)(i / 2);
}
