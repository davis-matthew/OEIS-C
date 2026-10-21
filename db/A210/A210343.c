/*
 * A210343  a(n) = Fibonacci(n+1)^n.
 * https://oeis.org/A210343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = fib(n+1)^n
 * Range:     u8 n <= 3, u16 n <= 5, u32 n <= 7, u64 n <= 9, u128 n <= 13,
 *            big any n
 * Helpers:   big_pow, fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = fib(n + 1);
    seq_t exp  = (seq_t)n;
    return big_pow(base, exp);
}
