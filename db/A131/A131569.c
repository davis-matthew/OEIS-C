/*
 * A131569  a(n) = (1/2)*(F(n+2)-1)*(F(n+2)-2) + F(n), where F() are the
 *          Fibonacci numbers.
 * https://oeis.org/A131569
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (1/2)*(F(n+2)-1)*(F(n+2)-2) + F(n)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t fn  = fib(n);       /* F(n)  */
    seq_t fn2 = fib(n + 2);   /* F(n+2) */

    seq_t t1 = fn2 - 1;
    seq_t t2 = fn2 - 2;

    /* Divide one of the factors by 2 to avoid overflow on 64‑bit seq_t */
    if ((t1 & 1) == 0)
        t1 = t1 / 2;
    else
        t2 = t2 / 2;

    seq_t prod = t1 * t2;
    return prod + fn;
}
