/*
 * A027785  a(n) = 6*(n+1)*binomial(n+2,12).
 * https://oeis.org/A027785
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 6*(n+1)*binomial(n+2,12)
 * Range:     u8 n <= 10, u16 n <= 13, u32 n <= 25, u64 n <= 126,
 *            u128 n <= 1000, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* The sequence starts at n = 10; smaller indices are
       outside the defined range and will simply return 0. */
    if (n < 10) {
        return 0;
    }

    /* Compute the binomial coefficient C(n+2,12). */
    seq_t bin = binomial(n + 2, 12);

    /* Compute the factor 6*(n+1).  The multiplication is done in
       the unsigned integer type of the result so that the final
       product is produced in seq_t. */
    seq_t factor = 6 * ((seq_t)n + 1);

    /* Return the product. */
    return factor * bin;
}
