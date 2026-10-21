/*
 * A200142  Number of near-matchings on the complete graph over 2n+1 vertices.
 * https://oeis.org/A200142
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (2n+1)! / (2^n * (n-1)!)
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 8, u64 n <= 15, u128 n <= 18,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)
        return 0;                           /* out of range, return 0 */

    idx_t rem = n;                          /* how many factors of 2 to divide out */
    seq_t res = 1;                          /* accumulate the product */

    idx_t upper = 2 * n + 1;                /* upper limit of the product */

    for (idx_t k = n; k <= upper; ++k) {
        res *= (seq_t)k;                    /* multiply by current factor */

        /* remove powers of two up to the remaining count */
        while (rem > 0 && !(res & 1)) {     /* res even? */
            res >>= 1;                     /* divide by 2 */
            --rem;
        }
    }

    /* Any remaining powers of two are removed in one shift */
    if (rem > 0)
        res >>= rem;

    return res;
}
