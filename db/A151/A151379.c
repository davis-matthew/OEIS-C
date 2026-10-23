/*
 * A151379  Number of walks within N^2 (the first quadrant of Z^2) starting at
 *          (0,0), ending on the vertical axis and consisting of 2 n steps
 *          taken from {(-1, 0), (1, -1), (1, 1)}.
 * https://oeis.org/A151379
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, walk, easy
 * Formula:   a(n) = Catalan(n) * binomial(n, floor(n/2))
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 13, u64 n <= 24, u128 n <= 46,
 *            big any n
 * Helpers:   binom, catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binom.h"
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* Compute the binomial coefficient \ell = \binom{n}{\lfloor n/2 \rfloor} */
    seq_t comb = binom(n, n / 2);

    /* Compute the nth Catalan number */
    seq_t cat = catalan(n);

    /* Multiply: a(n) = Catalan(n) * binomial(n, floor(n/2)) */
    return cat * comb;
}
