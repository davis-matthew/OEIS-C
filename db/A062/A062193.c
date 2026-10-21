/*
 * A062193  Fourth (unsigned) column sequence of triangle A062139 (generalized
 *          a=2 Laguerre).
 * https://oeis.org/A062193
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n+3)! * binom(n+5,5) / 6
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 7, u64 n <= 15, u128 n <= 28,
 *            big any n
 * Helpers:   binomial, factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute (n+3)! and reduce the 6‑fold factor first.
       The division is exact since (n+3)! always contains 3·2·1. */
    seq_t f = factorial(n + 3);
    f /= (seq_t)6;

    /* Compute the binomial coefficient C(n+5,5). */
    seq_t b = binomial(n + 5, 5);

    /* Final result: ( (n+3)! / 6 ) * C(n+5,5). */
    return f * b;
}
