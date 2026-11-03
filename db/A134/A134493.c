/*
 * A134493  a(n) = Fibonacci(6*n+1).
 * https://oeis.org/A134493
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(6*n+1)
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 30,
 *            big any n
 * Validated: standard, pass, 35 terms through n = 706 (2026-11-01)
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
    idx_t k = (idx_t)((( __int128 )6 * ( __int128 )n) + 1);
    return fib(k);
}
