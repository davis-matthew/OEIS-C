/*
 * A203680  v(n+1)/v(n), where v=A203679.
 * https://oeis.org/A203680
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Product_{i=1..n} (x_i^2 + x_i*x_{n+1} + x_{n+1}^2) with
 *            x_i = 2^(i-1); equivalently v(n+1)/v(n) where v = A203679
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 3, u64 n <= 5, u128 n <= 7,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>

seq_t a(idx_t n)
{
    seq_t y = 1;
    for (idx_t i = 0; i < n; ++i) y = y * 2;   /* y = 2^n */
    seq_t x = 1;
    seq_t res = 1;
    for (idx_t i = 1; i <= n; ++i)
    {
        res = res * (x * x + x * y + y * y);
        x = x * 2;
    }
    return res;
}
