/*
 * A011853  a(n) = floor( binomial(n,7)/7 ).
 * https://oeis.org/A011853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor( binomial(n,7)/7 )
 * Range:     u8 n <= 9, u16 n <= 15, u32 n <= 108, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    seq_t res = binomial(n, 7);   /* exact binomial coefficient */
    return res / 7;               /* integer division is floor() */
}
