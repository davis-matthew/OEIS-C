/*
 * A203847  a(n) = tau(n)*Fibonacci(n), where tau(n) = A000005(n), the number
 *          of divisors of n.
 * https://oeis.org/A203847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = tau(n)*Fibonacci(n)
 * Range:     u8 n <= 11, u16 n <= 21, u32 n <= 44, u64 n <= 89, u128 n <= 181,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include <stdint.h>

seq_t a(idx_t n)
{
    /* compute tau(n), the number of divisors of n */
    seq_t tau = 1;
    idx_t m = n;
    for (idx_t p = 2; p <= m / p; ++p)
    {
        if (m % p == 0)
        {
            idx_t cnt = 0;
            while (m % p == 0)
            {
                m /= p;
                cnt++;
            }
            tau *= (cnt + 1);
        }
    }
    if (m > 1)               /* remaining prime factor */
        tau *= 2;

    return tau * fib(n);     /* Fibonacci(n) is provided by utils/fib.h */
}
