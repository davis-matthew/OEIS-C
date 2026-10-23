/*
 * A102890  A102889(n) is at least a(n).
 * https://oeis.org/A102890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(3*n^(2/3)) = ceil(cuberoot(27*n^2))
 * Range:     u8 n <= 2, u16 n <= 40, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Helpers:   ceil_cube_root
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ceil_cube_root.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t m  = (seq_t)27 * nn;
    return ceil_cube_root(m);
}
