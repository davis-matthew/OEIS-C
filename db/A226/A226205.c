/*
 * A226205  a(n) = F(n)^2 - F(n-1)^2 or F(n+1) * F(n-2) where F(n) =
 *          A000045(n), the Fibonacci numbers.
 * https://oeis.org/A226205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, changed
 * Formula:   a(n) = F(n)^2 - F(n-1)^2 where F(0)=0, F(1)=1 are the Fibonacci
 *            numbers
 * Range:     u8 n <= 7, u16 n <= 13, u32 n <= 25, u64 n <= 48, u128 n <= 94,
 *            big any n
 * Helpers:   fib_pair_internal
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair_internal.h"
/* Implements fast doubling Fibonacci to compute F(n) and F(n+1) in
   logarithmic time. */

seq_t a(idx_t n)
{
    /* The sequence is defined for n >= 1, but the implementation
       accepts n == 0 as returning 0 for safety. */
    if (n == 0)
        return 0;

    seq_t fn, fn1;
    fib_pair_internal((idx_t)(n), &fn, &fn1);   /* fn  = F(n),   fn1 = F(n+1) */

    seq_t fnm1 = fn1 - fn;                       /* F(n-1) = F(n+1) - F(n) */

    return fn * fn - fnm1 * fnm1;                /* F(n)^2 - F(n-1)^2 */
}
