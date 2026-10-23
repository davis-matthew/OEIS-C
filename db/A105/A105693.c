/*
 * A105693  a(n) = Fibonacci(2n+2)-2^n.
 * https://oeis.org/A105693
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(2n+2) - 2^n
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 92,
 *            big any n
 * Helpers:   fib_fast_impl, pow2u
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fib_fast_impl.h"
#include "../utils/pow2u.h"

seq_t a(idx_t n) {
    seq_t fn, fn1;
    fib_fast_impl(2 * n + 2, &fn, &fn1);   /* fn = Fibonacci(2n+2) */
    seq_t pow = pow2u(n);
    return fn - pow;
}
