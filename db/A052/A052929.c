/*
 * A052929  Expansion of g.f. (2-3*x-x^2)/((1-x^2)*(1-3*x)).
 * https://oeis.org/A052929
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3^n + (1 + (-1)^n)/2; a(0)=2, a(1)=3, a(2)=10
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 20, u64 n <= 40, u128 n <= 80,
 *            big any n
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    seq_t term  = pow3(n);
    seq_t extra = ((n & 1) == 0) ? 1 : 0;
    return term + extra;
}
