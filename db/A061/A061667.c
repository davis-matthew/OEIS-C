/*
 * A061667  a(n) = Fibonacci(2*n+1) - 2^(n-1).
 * https://oeis.org/A061667
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(2*n+1) - 2^(n-1)
 * Range:     u8 n <= 6, u16 n <= 11, u32 n <= 23, u64 n <= 46, u128 n <= 92,
 *            big any n
 * Helpers:   fib, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/pow2.h"

seq_t a(idx_t n) {
    /* n >= 1 */
    idx_t fib_index = 2 * n + 1;
    seq_t fib_val = fib(fib_index);
    seq_t pow_val = pow2(n - 1);
    return fib_val - pow_val;
}
