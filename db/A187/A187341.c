/*
 * A187341  Floor((5-sqrt(5))n); complement of A187342.
 * https://oeis.org/A187341
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((5-sqrt(5)) * n)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 61, u64 n <= 61, u128 n <= 61,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* cast index to seq_t for multiplication */
    seq_t nn = (seq_t)n;

    /* compute floor_sqrt(5 * n * n) */
    seq_t t = 5 * nn * nn;
    seq_t sqrt_term = floor_sqrt(t);

    /* a(n) = 5n - floor(n * sqrt(5)) - 1, where
       floor(n * sqrt(5)) = floor_sqrt(5*n*n) */
    seq_t result = 5 * nn - sqrt_term - 1;
    return result;
}
