/*
 * A143250  Reverse binary expansion of the Fibonacci numbers.
 * https://oeis.org/A143250
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = Decimal(reverse(Binary(Fibonacci(n))))
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 40, u64 n <= 40, u128 n <= 40,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation: compute Fibonacci(n), reverse its binary expansion by
 * iterating from the least significant bit to the most significant bit
 * and accumulate the reversed number. */

#include "../utils/fib.h"

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
