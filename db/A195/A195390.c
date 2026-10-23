/*
 * A195390  a(n) = (6*n)!.
 * https://oeis.org/A195390
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (6*n)!
 * Range:     u8 n <= 0, u16 n <= 1, u32 n <= 2, u64 n <= 3, u128 n <= 5,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    idx_t m = 6 * n;
    return factorial(m);
}
