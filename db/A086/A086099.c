/*
 * A086099  a(n) = OR(k AND (n-k): 0<=k<=n), AND and OR bitwise.
 * https://oeis.org/A086099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, hear, look
 * Formula:   a(n) = OR{k & (n-k) : 0<=k<=n} Recurrence: a(0) = 0 a(2*m) = 2 *
 *            2^⌊log2(m)⌋ - 1 a(2*m+1) = 2 * a(m)
 * Range:     u8 n <= 511, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   a_calc
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t, builtins */
#include "../utils/a_calc.h"

seq_t a(idx_t n)
{
    return a_calc(n);
}
