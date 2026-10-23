/*
 * A032908  One of four 3rd-order recurring sequences for which the first
 *          derived sequence and the Galois transformed sequence coincide.
 * https://oeis.org/A032908
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  eigen, nonn, easy
 * Formula:   a(n) = Fibonacci(2*n-1) + 1 (a(0)=2)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 47, u128 n <= 93,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0 || n == 1)          /* a(0)=2, a(1)=2 */
        return (seq_t)2;
    /* For n>=2 : a(n) = Fib(2*n-1) + 1 */
    return fib(2*n - 1) + (seq_t)1;
}
