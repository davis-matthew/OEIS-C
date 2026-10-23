/*
 * A183356  One quarter the number of n X 4 1..4 arrays with no two neighbors
 *          of any element equal to each other.
 * https://oeis.org/A183356
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (12*fibonacci(n+1))^2 for n>1, a(1) = 36
 * Range:     u8 n <= 1, u16 n <= 7, u32 n <= 18, u64 n <= 41, u128 n <= 87,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    if (n == 1) return 36;               /* a(1) = 36 */

    /* compute Fibonacci(n+1) using the provided fast‑doubling routine */
    idx_t k = n + 1;                     /* ensure k is safe (n << 10000)   */
    seq_t f, fnext;                      /* f   = Fibonacci(k)              */
    fib_pair(k, &f, &fnext);             /* fib_pair returns (F(k),F(k+1)) */

    seq_t t = (seq_t)12 * f;             /* t = 12 * Fibonacci(n+1)         */
    return t * t;                        /* a(n) = t^2                       */
}
