/*
 * A130744  a(n) = n*(n+2)*n!.
 * https://oeis.org/A130744
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n*(n+2)*n!
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 10, u64 n <= 18, u128 n <= 32,
 *            big any n
 * Helpers:   factorial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/factorial.h"

seq_t a(idx_t n)
{
    /* Compute n! */
    seq_t fac = factorial(n);

    /* a(n) = n * (n+2) * n! */
    return (seq_t)n * ((seq_t)n + 2) * fac;
}
