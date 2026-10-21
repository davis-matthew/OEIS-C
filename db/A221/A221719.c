/*
 * A221719  a(n) = 3*2^n - Fibonacci(n+3) - 1.
 * https://oeis.org/A221719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*2^n - Fibonacci(n+3) - 1
 * Range:     u8 n <= 6, u16 n <= 14, u32 n <= 30, u64 n <= 62, u128 n <= 126,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    seq_t pow2n = pow2(n);                // 2^n
    seq_t result = pow2n + pow2n;         // 2*2^n
    result += pow2n;                      // 3*2^n
    seq_t fib_term = fib(n + 3);          // Fibonacci(n+3)
    result -= fib_term;
    result -= (seq_t)1;
    return result;
}
