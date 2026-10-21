/*
 * A087374  Smallest square >= n!.
 * https://oeis.org/A087374
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ceil(sqrt(n!))^2
 * Range:     u8 n <= 4, u16 n <= 7, u32 n <= 12, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   fact_upto, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fact_upto.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute n! */
    seq_t fact = fact_upto(n);

    /* Compute floor(sqrt(n!)) */
    seq_t root = floor_sqrt(fact);

    /* Square it */
    seq_t root_sq = root * root;

    /* If it's smaller than n!, increment root and square again */
    if (root_sq < fact) {
        root_sq = (root + 1) * (root + 1);
    }

    return root_sq;
}
