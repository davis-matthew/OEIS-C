/*
 * A033278  Number of diagonal dissections of an n-gon into 6 regions.
 * https://oeis.org/A033278
 *
 * Offset:    7    (a(7) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = binomial(n+4,5)*binomial(n-3,5)/6
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 30, u64 n <= 264,
 *            u128 n <= 1000, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computed for n >= 7; values for n < 7 are returned as 0. */

#include "../utils/binom.h"   /* provides seq_t binom(idx_t a, idx_t b) */

seq_t a(idx_t n)
{
    /* The sequence is defined only for n >= 7; for smaller n return 0. */
    if (n < 7)
        return 0;

    /* Compute the two binomial coefficients. */
    seq_t b1 = binom(n + 4, 5);
    seq_t b2 = binom(n - 3, 5);

    /* Divide the product by 6 by first removing factors 2 and 3
       from the individual terms. This guarantees that we never
       overflow 64‑bit intermediate results, while still working
       correctly for arbitrary‑precision seq_t types. */
    /* Divide by 2.  At least one of b1 or b2 is even. */
    if ((b1 & 1) == 0)
        b1 /= 2;
    else
        b2 /= 2;

    /* Divide by 3.  At least one of b1 or b2 is now a multiple of 3. */
    if (b1 % 3 == 0)
        b1 /= 3;
    else
        b2 /= 3;

    /* The remaining product is exactly a(n). */
    return b1 * b2;
}
