/*
 * A192746  Constant term of the reduction by x^2 -> x+1 of the polynomial
 *          p(n,x) defined below in Comments.
 * https://oeis.org/A192746
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 4*fib(n+2)-3
 * Range:     u8 n <= 8, u16 n <= 19, u32 n <= 42, u64 n <= 88, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Uses the helper function fib(idx_t n) from ../utils/fib.h  */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t f = fib(n + 2);
    return f * (seq_t)4 - (seq_t)3;
}
