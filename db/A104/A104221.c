/*
 * A104221  a(n) = Fibonacci(n) - (Fibonacci(n) mod 2).
 * https://oeis.org/A104221
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Fibonacci(n) - (Fibonacci(n) mod 2) (OEIS A104221)
 * Range:     u8 n <= 13, u16 n <= 24, u32 n <= 47, u64 n <= 93, u128 n <= 186,
 *            big any n
 * Helpers:   fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_fast.h"

seq_t a(idx_t n)
{
    seq_t f, fn;               /* f = Fib(n), fn = Fib(n+1) */
    fib_fast(n, &f, &fn);      /* fast doubling, works for all seq_t types */
    seq_t r = f % ((seq_t)2);   /* parity of Fib(n) */
    return f - r;              /* clear the least significant bit if odd */
}
