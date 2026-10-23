/*
 * A034744  Dirichlet convolution of Fibonacci numbers with themselves.
 * https://oeis.org/A034744
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = sum_{d|n} Fibonacci(d) * Fibonacci(n/d)
 * Range:     u8 n <= 11, u16 n <= 23, u32 n <= 46, u64 n <= 92, u128 n <= 184,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t sum = 0;
    for (idx_t d = 1; d <= n / d; ++d) {
        if (n % d == 0) {
            seq_t fd = fib(d);
            idx_t e = n / d;
            seq_t fe = fib(e);
            seq_t prod = fd * fe;
            sum += prod;
            if (d != e) {
                sum += prod;   /* include the term for the complementary divisor */
            }
        }
    }
    return sum;
}
