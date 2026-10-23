/*
 * A163250  a(n) = A000045(n+6) - (n^2 + 4*n + 8).
 * https://oeis.org/A163250
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = F(n+6) - (n^2 + 4*n + 8)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 41, u64 n <= 87, u128 n <= 180,
 *            big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib.h"

seq_t a(idx_t n)
{
    /* Compute the Fibonacci number F(n+6). */
    seq_t fn = fib(n + 6);

    /* Promote the index to seq_t for arithmetic. */
    seq_t ns = (seq_t)n;

    /* Compute n^2 + 4*n + 8. */
    seq_t sub = ns * ns + ns * 4 + 8;

    /* Return the difference. */
    return fn - sub;
}
