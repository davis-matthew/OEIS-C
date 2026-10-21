/*
 * A230030  Numbers k not divisible by 5 such that k^2 written in base 5 has
 *          no digit > 1.
 * https://oeis.org/A230030
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, more
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 3, u64 n <= 8, u128 n <= 11,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation of OEIS A230030.
   a(1) = 1
   a(2) = 972799
   a(3) = 3051273374
   a(4) = 6132750376
   a(5) = 839228035909
   For n >= 6, a(n) = P(5^(n-2)) where
        P(x) = 25x^4 + 15x^3 - 4x^2 + 3x + 1   (x not divisible by 5).
   The formula is applied directly, keeping all arithmetic in seq_t. */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* First five terms are taken as constants. */
    if (n == 1) return (seq_t)1;
    if (n == 2) return (seq_t)972799;
    if (n == 3) return (seq_t)3051273374;
    if (n == 4) return (seq_t)6132750376;
    if (n == 5) return (seq_t)839228035909;

    /* For n >= 6 use the polynomial evaluated at 5^(n-2). */
    /* x = 5^(n-2) */
    seq_t exp = (seq_t)(n - 2);
    seq_t x   = big_pow((seq_t)5, exp);

    /* Compute successive powers of x. */
    seq_t x2  = x * x;
    seq_t x3  = x2 * x;
    seq_t x4  = x3 * x;

    /* Evaluate P(x) = 25*x^4 + 15*x^3 - 4*x^2 + 3*x + 1. */
    seq_t term = (seq_t)25 * x4;
    term += (seq_t)15 * x3;
    term += (seq_t)3 * x;
    term += (seq_t)1;
    term -= (seq_t)4 * x2;

    return term;
}
