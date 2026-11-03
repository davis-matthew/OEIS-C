/*
 * A033890  a(n) = Fibonacci(4*n + 2).
 * https://oeis.org/A033890
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = Fibonacci(4*n+2)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 11, u64 n <= 22, u128 n <= 46,
 *            big any n
 * Validated: standard, pass, 38 terms through n = 979 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

static seq_t fib(idx_t n) {
    if (n <= 2) return (seq_t)1;
    seq_t f1 = 1, f2 = 1, f3;
    for (idx_t i = 3; i <= n; ++i) {
        f3 = f1 + f2;
        f1 = f2;
        f2 = f3;
    }
    return f2;
}

seq_t a(idx_t n)
{
    /* Compute 4*n+2 using 128-bit to avoid wrapping for realistic n */
    __uint128_t tmp = ( __uint128_t )n * 4 + 2;
    idx_t k = (idx_t)tmp;   /* 4*n+2 fits for n up to 10000 */
    return fib(k);
}
