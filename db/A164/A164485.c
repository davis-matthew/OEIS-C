/*
 * A164485  Number of binary strings of length n with no substrings equal to
 *          0001, 1000 or 1001.
 * https://oeis.org/A164485
 *
 * Offset:    4    (a(4) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 4 * Fib(n) + 1 for n ≥ 4 (Fib is the standard Fibonacci
 *            sequence with Fib(0)=0, Fib(1)=1)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* The offset is 4.  Values below the offset are irrelevant. */
    if (n < 4)
        return 0;

    seq_t f = fib(n);   /* Fib(n) */
    return f * 4 + 1;   /* 4*Fib(n)+1 */
}
