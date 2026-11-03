/*
 * A222556  Number of (n+2)X1 arrays of occupancy after each element moves up
 *          to +-n places but not 0.
 * https://oeis.org/A222556
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = binom(2 n + 3, n + 1) - (n + 1)(n + 3)
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 9, u64 n <= 9, u128 n <= 9,
 *            big any n
 * Validated: partial, pass, 9 terms through n = 9 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Binomial coefficient C(2 n + 3, n + 1) */
    seq_t res = binomial(2*n + 3, n + 1);
    /* Subtract (n + 1)(n + 3) */
    seq_t sub = (seq_t)(n + 1) * (seq_t)(n + 3);
    return res - sub;
}
