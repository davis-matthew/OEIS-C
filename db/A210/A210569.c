/*
 * A210569  a(n) = (n-3)*(n-2)*(n-1)*n*(n+1)/30.
 * https://oeis.org/A210569
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n-3)*(n-2)*(n-1)*n*(n+1)/30
 * Range:     u8 n <= 7, u16 n <= 18, u32 n <= 166, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n < 4)
        return 0;
    /* Equivalent form: 4 * binomial(n+1, 5) */
    return 4 * binomial(n + 1, 5);
}
