/*
 * A080097  a(n) = Fibonacci(n+2)^2 - 1.
 * https://oeis.org/A080097
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = Fibonacci(n+2)^2 - 1
 * Range:     u8 n <= 5, u16 n <= 11, u32 n <= 22, u64 n <= 45, u128 n <= 91,
 *            big any n
 * Helpers:   fib_fast
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_fast.h"

seq_t a(idx_t n)
{
    idx_t target = n + 2;              /* we need Fibonacci(n+2)   */
    seq_t fn, fn_next;
    fib_fast(target, &fn, &fn_next);   /* fn = F(n+2)            */
    seq_t res = fn * fn;               /* square the Fibonacci    */
    res -= 1;                          /* subtract one            */
    return res;
}
