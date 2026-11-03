/*
 * A102890  A102889(n) is at least a(n).
 * https://oeis.org/A102890
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = ceil(3*n^(2/3)) = ceil(cuberoot(27*n^2))
 * Range:     u8 n <= 3, u16 n <= 49, u32 n <= 71, u64 n <= 71, u128 n <= 71,
 *            big any n
 * Validated: partial, pass, 71 terms through n = 71 (2026-11-01)
 * Helpers:   icbrt_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/icbrt_seq.h"

static seq_t ceil_cube_root(seq_t x)
{
    seq_t r = icbrt_seq(x);
    if (r * r * r < x) {
        return r + 1;
    }
    return r;
}

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n * (seq_t)n;
    seq_t m  = (seq_t)27 * nn;
    return ceil_cube_root(m);
}
