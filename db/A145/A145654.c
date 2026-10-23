/*
 * A145654  Partial sums of A000918, starting from index 1.
 * https://oeis.org/A145654
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2 * (2^n - n - 1)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <assert.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    assert(n >= 1);
    seq_t t = pow2(n);
    t -= (seq_t)n;
    t -= 1;
    return t * 2;
}
