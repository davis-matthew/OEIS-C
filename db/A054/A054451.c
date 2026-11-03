/*
 * A054451  Third column of triangle A054450 (partial row sums of unsigned
 *          Chebyshev triangle A049310).
 * https://oeis.org/A054451
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = F(2*k+3)-1 if n = 2*k; * a(n) = F(2*(k+2))-(k+2) if n =
 *            2*k+1 * (Fibonacci numbers F(1)=1,F(2)=1…) from seq_t fib(idx_t
 *            n)
 * Range:     u8 n <= 10, u16 n <= 21, u32 n <= 44, u64 n <= 90, u128 n <= 183,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 870 (2026-11-01)
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
    if ((n & 1) == 0) {                 /* even n = 2*k */
        idx_t k = n >> 1;
        seq_t f = fib(2 * k + 3);
        return f - 1;
    } else {                            /* odd n = 2*k+1 */
        idx_t k = n >> 1;
        idx_t m = k + 2;
        seq_t f = fib(2 * m);
        return f - (seq_t)m;
    }
}
