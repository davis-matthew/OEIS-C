/*
 * A020712  Pisot sequences E(5,8), P(5,8).
 * https://oeis.org/A020712
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(n+5)
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    return fib(n + 5);
}
