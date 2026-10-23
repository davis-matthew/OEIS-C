/*
 * A023536  Convolution of natural numbers with A023532.
 * https://oeis.org/A023536
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (n+1)(n+2)/2 - Sum_{k=1..n+1} floor((sqrt(8k+1)-1)/2)
 * Range:     u8 n <= 6, u16 n <= 378, u32 n <= 500, u64 n <= 500,
 *            u128 n <= 500, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Integral arithmetic only – all intermediate results are of type seq_t. */
    seq_t total = ((seq_t)(n + 1) * ((seq_t)n + 2)) / 2;

    seq_t sum_floor = 0;
    for (idx_t k = 1; k <= n + 1; ++k) {
        /* floor( (sqrt(8k+1)-1)/2 )  =  ( floor(sqrt(8k+1)) - 1 ) / 2  */
        seq_t t = floor_sqrt((seq_t)(8 * k + 1));
        seq_t term = (t - 1) / 2;
        sum_floor += term;
    }

    return total - sum_floor;
}
