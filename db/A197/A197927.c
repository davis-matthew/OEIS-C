/*
 * A197927  The number of isolated nodes in all labeled directed graphs (with
 *          self loops allowed) on n nodes.
 * https://oeis.org/A197927
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * 2^((n-1)^2)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 6, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;
    idx_t nm1 = n - 1;
    idx_t exp = nm1 * nm1;
    seq_t v = pow2(exp);
    return (seq_t)n * v;
}
