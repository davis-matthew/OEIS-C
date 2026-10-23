/*
 * A033890  a(n) = Fibonacci(4*n + 2).
 * https://oeis.org/A033890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Fibonacci(4*n+2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute 4*n+2 using 128-bit to avoid wrapping for realistic n */
    __uint128_t tmp = ( __uint128_t )n * 4 + 2;
    idx_t k = (idx_t)tmp;   /* 4*n+2 fits for n up to 10000 */
    return fib(k);
}
