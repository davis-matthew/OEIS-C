/*
 * A196662  Expansion of g.f. (1-3*x)/(1-10*x).
 * https://oeis.org/A196662
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(0) = 1, a(n) = 7*10^(n-1) for n>0 (equivalently a(n) =
 *            10*a(n-1) with initial term 1)
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 19, u128 n <= 20,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    if (n == 0) return 1;
    return 7 * pow10(n - 1);
}
