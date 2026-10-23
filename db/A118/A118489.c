/*
 * A118489  Squares for which the product of the digits is a triangular
 *          number.
 * https://oeis.org/A118489
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = the n-th square whose digit product is a triangular
 *            number
 * Range:     u8 n <= 2, u16 n <= 29, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/floor_sqrt.h"

/* Check whether x is a triangular number. */
static bool is_triangular(seq_t x)
{
    seq_t z = 8 * x + 1;
    seq_t s = floor_sqrt(z);
    return (s * s == z) && ((s & 1) != 0);
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0;; ++k)
    {
        seq_t sq = (seq_t)k * (seq_t)k;
        seq_t prod = digit_product(sq);
        if (is_triangular(prod))
        {
            ++count;
            if (count == n)
                return sq;
        }
    }
    return 0; /* Should never reach here for n ≥ 1. */
}
