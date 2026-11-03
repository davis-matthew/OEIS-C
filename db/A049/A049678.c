/*
 * A049678  a(n) = F(8*n+4)/3, where F=A000045 (the Fibonacci sequence).
 * https://oeis.org/A049678
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = F(8*n+4)/3, where F is the Fibonacci sequence
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 5, u64 n <= 11, u128 n <= 22,
 *            big any n
 * Validated: standard, pass, 32 terms through n = 559 (2026-11-01)
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
    seq_t val = fib(8*n + 4);   /* F(8n+4) */
    val /= 3;                   /* exact division */
    return val;
}
