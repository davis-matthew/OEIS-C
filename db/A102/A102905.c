/*
 * A102905  a(n) = A113655(Fibonacci(n+1)).
 * https://oeis.org/A102905
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = A113655(Fibonacci(n+1)), where A113655(m) = m-2 if m mod
 *            3 = 0, m+2 if m mod 3 = 1, m otherwise (m mod 3 = 2)
 * Range:     u8 n <= 12, u16 n <= 23, u32 n <= 37, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_fast.h"

seq_t a(idx_t n)
{
    seq_t f, f_next;
    /* Compute Fibonacci(n+1). */
    fib_fast(n + 1, &f, &f_next);

    /* Apply the A113655 transformation. */
    idx_t r = (idx_t)(f % 3);
    if (r == 0)
        f -= 2;
    else if (r == 1)
        f += 2;

    return f;
}
