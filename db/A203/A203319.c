/*
 * A203319  a(n) = n*Fibonacci(n) * Sum_{d|n} 1/(d*Fibonacci(d)).
 * https://oeis.org/A203319
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n*Fibonacci(n) * Sum_{d|n} 1/(d*Fibonacci(d)) Equivalent
 *            integer implementation: a(n) = Σ_{d|n} (n/d) *
 *            (Fibonacci(n)/Fibonacci(d)) because Fibonacci(d) | Fibonacci(n)
 *            whenever d | n
 * Range:     u8 n <= 7, u16 n <= 17, u32 n <= 39, u64 n <= 83, u128 n <= 175,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib.h"   /* provides seq_t fib(idx_t n) */

seq_t a(idx_t n) {
    if (n == 0) return 0;          /* offset starts at 1; unused for 0 */
    seq_t fib_n = fib(n);
    seq_t sum = 0;

    for (idx_t i = 1; i * i <= n; ++i) {
        if (n % i == 0) {
            idx_t d1 = i;
            seq_t term1 = ((seq_t)(n / d1)) * (fib_n / fib(d1));
            sum += term1;

            idx_t d2 = n / i;
            if (d2 != d1) {
                seq_t term2 = ((seq_t)(n / d2)) * (fib_n / fib(d2));
                sum += term2;
            }
        }
    }
    return sum;
}
