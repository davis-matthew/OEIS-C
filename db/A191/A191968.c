/*
 * A191968  a(n) = Fibonacci(8n+5) mod Fibonacci(8n+1).
 * https://oeis.org/A191968
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = Lucas(8*n - 1) Recurrence: a(1) = 29, a(2) = 1364, a(n) =
 *            47*a(n-1) - a(n-2) for n>2
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 5, u64 n <= 11, u128 n <= 23,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    /* offset = 1; n >= 1 */
    idx_t m = 8*n - 1;          /* index for Lucas number */
    seq_t fn, fn1;
    fib_pair(m, &fn, &fn1);     /* fn  = F(m),  fn1 = F(m+1) */
    /* Lucas(m) = 2*F(m+1) - F(m) */
    seq_t lucas = fn1 + fn1;    /* 2*F(m+1) */
    lucas = lucas - fn;         /* subtract F(m) */
    return lucas;
}
