/*
 * A100204  Numbers of cubes in the range [n^2, (n+1)^2].
 * https://oeis.org/A100204
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = number of integers m such that m^3 lies in [n^2, (n+1)^2]
 * Range:     u8 n <= 13, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   ceil_cube_root, cube_root_floor
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/ceil_cube_root.h"
#include "../utils/cube_root_floor.h"

seq_t a(idx_t n)
{
    /* compute n^2 and (n+1)^2 as seq_t */
    seq_t nsq  = (seq_t)n * (seq_t)n;
    seq_t n1sq = (seq_t)(n + 1) * (seq_t)(n + 1);

    /* lower bound: first integer m with m^3 >= n^2 */
    seq_t lower = ceil_cube_root(nsq);

    /* upper bound: last integer m with m^3 <= (n+1)^2 */
    seq_t upper = cube_root_floor(n1sq);

    if (upper < lower) {
        return 0;
    }
    return upper - lower + 1;
}
