/*
 * A031506  Number of consecutive integers placed in n bins under a certain
 *          packing scheme.
 * https://oeis.org/A031506
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(2*n) + Lucas(2*n+1) - 1 for n > 0 ( Lucas(m) =
 *            F_{m-1}+F_{m+1} ); a(0)=1
 * Range:     u8 n <= 5, u16 n <= 10, u32 n <= 22, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return (seq_t)1;

    seq_t f2n   = fib((idx_t)(2 * n));
    seq_t f2np2 = fib((idx_t)(2 * n + 2));

    return (f2n * (seq_t)2 + f2np2) - (seq_t)1;
}
