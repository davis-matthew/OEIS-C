/*
 * A054071  Position of 1 in the permutation of 1,2,...,n obtained by ordering
 *          the fractional parts {h*sqrt(2)} for h=1,2,...,n.
 * https://oeis.org/A054071
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 1 + ⌊n*(√2 – 1)⌋, for n ≥ 1 The integer part is computed
 *            by the equivalent inequality 2*k² ≤ (n – k)², which is solved by
 *            binary search
 * Range:     u8 n <= 67, u16 n <= 67, u32 n <= 67, u64 n <= 67, u128 n <= 67,
 *            big any n
 * Helpers:   floor_n_sqrt2_minus1
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_n_sqrt2_minus1.h"

seq_t a(idx_t n)
{
    if (n == 0)          /* undefined for n<1: any value is acceptable */
        return 0;

    seq_t val = (seq_t)floor_n_sqrt2_minus1(n);
    return val + 1;      /* a(n) = 1 + floor(n*(√2 – 1)) */
}
