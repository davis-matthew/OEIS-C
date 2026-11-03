/*
 * A143250  Reverse binary expansion of the Fibonacci numbers.
 * https://oeis.org/A143250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Decimal(reverse(Binary(Fibonacci(n))))
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Validated: partial, pass, 40 terms through n = 40 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute Fibonacci(n), reverse its binary expansion by
 * iterating from the least significant bit to the most significant bit
 * and accumulate the reversed number. */

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
    seq_t f = fib(n);          /* Fibonacci number F(n) */
    seq_t rev = 0;

    /* Reverse bits of f.  We only process until f becomes zero,
     * which skips leading zeros in the original representation. */
    while (f != 0)
    {
        rev = (rev << 1) | (f & 1);
        f >>= 1;
    }

    return rev;
}
