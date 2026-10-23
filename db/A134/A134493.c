/*
 * A134493  a(n) = Fibonacci(6*n+1).
 * https://oeis.org/A134493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(6*n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    idx_t k = (idx_t)((( __int128 )6 * ( __int128 )n) + 1);
    return fib(k);
}
