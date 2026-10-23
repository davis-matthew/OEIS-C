/*
 * A183872  n+floor(sqrt(5n)); complement of A183873.
 * https://oeis.org/A183872
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(5*n))
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   floor_sqrt5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt5.h"

seq_t a(idx_t n)
{
    seq_t n_seq   = (seq_t)n;
    seq_t sqrt5n  = floor_sqrt5(n);
    return n_seq + sqrt5n;
}
