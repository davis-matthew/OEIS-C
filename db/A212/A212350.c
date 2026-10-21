/*
 * A212350  Maximal number of "good" manifolds in an n-serial polytope.
 * https://oeis.org/A212350
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 6 (n=1), 8 (n=2), 12 (n=3), 20 (n=4), 2^n (n>=5)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 33, u128 n <= 33,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    if (n == 1) return (seq_t)6;
    if (n == 2) return (seq_t)8;
    if (n == 3) return (seq_t)12;
    if (n == 4) return (seq_t)20;
    return pow2(n);
}
