/*
 * A205963  a(n) = Fibonacci(n)*A000118(n) for n>=1 with a(0)=1, where
 *          A000118(n) is the number of ways of writing n as a sum of 4
 *          squares.
 * https://oeis.org/A205963
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Fibonacci(n)*A000118(n) for n>=1, a(0)=1 * where
 *            A000118(n) = 8 * sum_{d|n, 4∤d} d
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 35, u64 n <= 80, u128 n <= 169,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)1;          /* a(0) = 1 */

    seq_t fn = fib(n);                    /* Fibonacci(n)   */
    seq_t sum = 0;                        /* sum of divisors not divisible by 4 */

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            if (d1 % 4u != 0) sum += (seq_t)d1;

            idx_t d2 = n / i;
            if (d2 != d1 && (d2 % 4u != 0)) sum += (seq_t)d2;
        }
    }

    return fn * sum * (seq_t)8;
}
