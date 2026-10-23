/*
 * A027934  a(0)=0, a(1)=1, a(2)=2; for n > 2, a(n) = 3*a(n-1) - a(n-2) -
 *          2*a(n-3).
 * https://oeis.org/A027934
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 2^n - Fibonacci(n+1)
 * Range:     u8 n <= 8, u16 n <= 16, u32 n <= 32, u64 n <= 64, u128 n <= 128,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2.h"
#include "../utils/fib.h"

seq_t a(idx_t n) {
    seq_t two_pow = pow2(n);          /* compute 2^n   */
    seq_t fib_val = fib(n + 1);     /* compute Fibonacci(n+1) */
    return two_pow - fib_val;       /* a(n) = 2^n - Fibonacci(n+1) */
}
