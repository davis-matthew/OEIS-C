/*
 * A194847  Write n = C(i,3)+C(j,2)+C(k,1) with i>j>k>=0; sequence gives i
 *          values.
 * https://oeis.org/A194847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = i where n = C(i,3)+C(j,2)+C(k,1) with i>j>k>=0. The
 *            representation is unique by the combinatorial number system of
 *            degree 3
 * Range:     u8 n <= 83, u16 n <= 123, u32 n <= 123, u64 n <= 123,
 *            u128 n <= 123, big any n
 * Helpers:   choose2_idx, choose3_idx, max_i_idx, max_j_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/choose2_idx.h"
#include "../utils/choose3_idx.h"
#include "../utils/max_i_idx.h"
#include "../utils/max_j_idx.h"

seq_t a(idx_t n)
{
    idx_t i   = max_i_idx(n);
    idx_t rem = n - choose3_idx(i);

    idx_t j = max_j_idx(rem, i);
    (void)j;  /* k is not needed for the output */

    return (seq_t)i;
}
