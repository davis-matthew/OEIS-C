/*
 * A110224  a(n) = Fibonacci(n)^3 + Fibonacci(n+1)^3.
 * https://oeis.org/A110224
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n)^3 + Fibonacci(n+1)^3
 * Range:     u8 n <= 4, u16 n <= 8, u32 n <= 15, u64 n <= 31, u128 n <= 61,
 *            big any n
 * Helpers:   cube, fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_fast.h"
#include "../utils/cube.h"

seq_t a(idx_t n)
{
    seq_t f, g;
    fib_fast(n, &f, &g);       /* f = Fib(n), g = Fib(n+1) */
    return cube(f) + cube(g);  /* f^3 + g^3 */
}
