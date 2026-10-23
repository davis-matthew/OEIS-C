/*
 * A022402  Fibonacci sequence beginning 1, 32.
 * https://oeis.org/A022402
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 31*A000045(n) + A000045(n+1) ; Recurrence: a(n) = a(n-1)
 *            + a(n-2), a(0) = 1, a(1) = 32
 * Range:     u8 n <= 5, u16 n <= 17, u32 n <= 40, u64 n <= 86, u128 n <= 178,
 *            big any n
 * Helpers:   fib_pair, mul_small
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"
#include "../utils/mul_small.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);          /* fn  = F(n),   fn1 = F(n+1) */
    return fn1 + mul_small(fn, 31);   /* 31*F(n) + F(n+1) */
}
