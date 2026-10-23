/*
 * A018910  Pisot sequence L(4,5).
 * https://oeis.org/A018910
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fib(n+3) + 2
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    return fib(n + (idx_t)3) + (seq_t)2;
}
