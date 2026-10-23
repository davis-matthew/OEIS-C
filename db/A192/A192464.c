/*
 * A192464  Constant term of the reduction by x^2->x+1 of the polynomial
 *          p(n,x) = 1 + x^n + x^(2n).
 * https://oeis.org/A192464
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 - Fib(n) + Fib(n+1) - Fib(2n) + Fib(2n+1)
 * Range:     u8 n <= 7, u16 n <= 12, u32 n <= 24, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   fib_pair
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_pair.h"

seq_t a(idx_t n)
{
    /* Compute Fib(n) and Fib(n+1) */
    seq_t fn, fn1;
    fib_pair(n, &fn, &fn1);              /* fn = F(n), fn1 = F(n+1) */

    /* Fast doubling formulas for Fib(2n) and Fib(2n+1) */
    seq_t two_fn1_minus_fn = ((seq_t)2 * fn1) - fn;  /* 2*F(n+1) - F(n) >= 0 */
    seq_t f2n  = fn * two_fn1_minus_fn;              /* F(2n)   = F(n) * (2*F(n+1)-F(n)) */
    seq_t f2n1 = fn1 * fn1 + fn * fn;                /* F(2n+1) = F(n+1)^2 + F(n)^2 */

    /* Compute the final expression without underflow */
    seq_t term1 = fn1 - fn;              /* F(n+1) - F(n) >= 0 */
    seq_t term2 = f2n1 - f2n;             /* F(2n+1) - F(2n) = F(2n-1) >= 0 */

    seq_t result = term1 + term2 + (seq_t)1;   /* a(n) = 1 + term1 + term2 */
    return result;
}
