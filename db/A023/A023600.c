/*
 * A023600  Convolution of A023532 and (F(2), F(3), F(4), ...).
 * https://oeis.org/A023600
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = fib(n+3) - 2 - Σ_{k≥1, k(k+3)/2 ≤ n} fib(n - k(k+3)/2 +
 *            2)
 * Range:     u8 n <= 10, u16 n <= 22, u32 n <= 32, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Validated: partial, pass, 32 terms through n = 32 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

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
#include <stdint.h>

seq_t a(idx_t n)
{
    /* initial value: sum of all Fibonacci numbers from F(2) to F(n+1) */
    seq_t result = fib(n + 3);      /* F(n+3) */
    result -= (seq_t)2;             /* subtract 2  (F(0)+F(1)) */

    /* subtract contributions of the indices where A023532 has a 0.
       These indices are m = k(k+3)/2, k=1,2,3,... */
    for (idx_t k = 1; ; ++k) {
        idx_t m = k * (k + 3) / 2;   /* m ≤ n ? */
        if (m > n)
            break;
        /* the corresponding Fibonacci term in the convolution */
        result -= fib(n - m + 2);
    }
    return result;
}
