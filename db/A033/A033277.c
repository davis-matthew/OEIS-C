/*
 * A033277  Number of diagonal dissections of an n-gon into 5 regions.
 * https://oeis.org/A033277
 *
 * Offset:    6    (a(6) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+3,4) * binomial(n-3,4) / 5
 * Range:     u8 n <= 6, u16 n <= 12, u32 n <= 35, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The sequence is defined for n >= 6.  For n < 6 the function returns 0. */
#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* For n < 6 a(6) is the first defined term (a(6)=0). */
    if (n < 6)
        return (seq_t)0;

    seq_t b1 = binomial(n + 3U, (idx_t)4);
    seq_t b2 = binomial(n - 3U, (idx_t)4);

    /* Divide by 5 before multiplication to keep intermediate values
       within the range that fits into seq_t when the result does.  
       Since 5 is prime only one of the binomial factors is guaranteedly
       divisible by 5; we test each and divide accordingly. */
    if ((b1 % (seq_t)5) == 0)
        b1 /= (seq_t)5;
    else
        b2 /= (seq_t)5;

    return b1 * b2;
}
