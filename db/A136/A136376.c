/*
 * A136376  a(n) = n*F(n) + (n-1)*F(n-1).
 * https://oeis.org/A136376
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*F(n) + (n-1)*F(n-1)
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 39, u64 n <= 83, u128 n <= 174,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t fn   = fib(n);
    seq_t fn1  = fib(n-1);
    return (seq_t)n * fn + (seq_t)(n-1) * fn1;
}
