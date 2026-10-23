/*
 * A183569  n+floor(sqrt(4n-3)), complement of A024206.
 * https://oeis.org/A183569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(4*n - 3))
 * Range:     u8 n <= 16, u16 n <= 113, u32 n <= 113, u64 n <= 113,
 *            u128 n <= 113, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t val = (seq_t)n;
    seq_t tmp = val * 4 - 3;
    seq_t sq  = floor_sqrt(tmp);
    return val + sq;
}
