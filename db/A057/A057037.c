/*
 * A057037  Let R(i,j) be the rectangle with antidiagonals 1; 2,3; 4,5,6; ...;
 *          each k is an R(i(k),j(k)) and A057037(n)=j(2n-1).
 * https://oeis.org/A057037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j(2n-1) where j(i,j) is the column coordinate of the
 *            entry numbered i in the rectangle whose antidiagonals are 1;
 *            2,3; 4,5,6; …, i.e. index(i,j) = (i+j-2)(i+j-1)/2 + i
 * Range:     u8 n <= 80, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   diag_of_idx, j_from_index
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/diag_of_idx.h"
#include "../utils/j_from_index.h"

seq_t a(idx_t n)
{
    /* compute a(n) = j(2n-1) */
    unsigned __int128 N128 = ((unsigned __int128)n << 1) - 1; /* 2n-1 */
    idx_t N = (idx_t)N128;                                   /* fits in idx_t */
    return j_from_index(N);
}
