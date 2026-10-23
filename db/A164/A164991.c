/*
 * A164991  Number of triangular involutions of n. A triangular involution is
 *          a square involution with at most three faces.
 * https://oeis.org/A164991
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 1 for n = 1 * * a(n) = 2^(n-1) - binomial(n-2,
 *            floor((n-2)/2)) * for n > 1 * * This uses the supplied utilities
 *            pow2 and binomial
 * Range:     u8 n <= 9, u16 n <= 17, u32 n <= 30, u64 n <= 30, u128 n <= 30,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n <= 1)
        return 1;                     /* a(1) = 1 (offset is 1) */

    /* Compute 2^(n-1) */
    seq_t p = pow2(n - 1);

    /* Compute binomial(n-2, floor((n-2)/2)) */
    idx_t k = (n - 2) / 2;             /* floor division */
    seq_t b = binomial(n - 2, k);

    return p - b;
}
