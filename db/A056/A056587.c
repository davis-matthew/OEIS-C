/*
 * A056587  Tenth power of Fibonacci numbers A000045.
 * https://oeis.org/A056587
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(n)^10, where F(n) is the n-th Fibonacci number
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 6, u64 n <= 10, u128 n <= 20,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);   /* fn = F(n), fn1 = F(n+1) */
    seq_t res = 1;
    for (int i = 0; i < 10; ++i)
        res *= fn;
    return res;
}
