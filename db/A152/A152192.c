/*
 * A152192  a(n) = Product_{k=1..floor((n-1)/2)} (1 + 4*cos(2*Pi*k/n)^2).
 * https://oeis.org/A152192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   * a(0) = 1 * for n > 0: * if n is odd: a(n) = Fib(n) * if n is
 *            even: a(n) = Fib(n/2)^2
 * Range:     u8 n <= 14, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 41 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
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
    if (n == 0) return (seq_t)1;
    if (n & 1) /* odd */
        return fib(n);
    else {      /* even */
        seq_t f = fib(n / 2);
        return f * f;
    }
}
