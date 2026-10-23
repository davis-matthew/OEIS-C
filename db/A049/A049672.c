/*
 * A049672  a(n) = (F(4*n) - F(n))/2, where F=A000045 (the Fibonacci
 *          sequence).
 * https://oeis.org/A049672
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (F(4*n) - F(n))/2
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 11, u64 n <= 23, u128 n <= 46,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t fn  = fib(n);
    seq_t f4n = fib(n * 4);
    return (f4n - fn) / 2;
}
