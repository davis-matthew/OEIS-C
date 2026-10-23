/*
 * A022350  Fibonacci sequence beginning 0, 16.
 * https://oeis.org/A022350
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 16 * A000045(n) = 16 * F(n)
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 42, u64 n <= 88, u128 n <= 180,
 *            big any n
 * Helpers:   fast_fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fast_fib.h"

seq_t a(idx_t n)
{
    seq_t fn, fn1;
    fast_fib(n, &fn, &fn1);
    fn *= (seq_t)16;          /* a(n) = 16 * F(n) */
    return fn;
}
