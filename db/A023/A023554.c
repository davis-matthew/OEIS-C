/*
 * A023554  Convolution of natural numbers >= 3 and (Fib(2), Fib(3), Fib(4),
 *          ...).
 * https://oeis.org/A023554
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n+5) + 2*Fibonacci(n+3) - (2*n + 9)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 179,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* for n = 0 the sequence is undefined – return 0 (behaviour below
     * the offset is irrelevant). */
    if (n == 0)
        return 0;

    seq_t f5 = fib(n + 5);
    seq_t f3 = fib(n + 3);

    seq_t res = f5;
    res += (seq_t)2 * f3;
    res -= ((seq_t)2 * (seq_t)n) + (seq_t)9;

    return res;
}
