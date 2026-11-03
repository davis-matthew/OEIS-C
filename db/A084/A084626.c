/*
 * A084626  a(n) = floor(C(n+6,6)/C(n+2,2)).
 * https://oeis.org/A084626
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(binomial(n+6,4) / 15)
 * Range:     u8 n <= 4, u16 n <= 30, u32 n <= 602, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 63 terms through n = 975 (2026-11-01)
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Compute C(n+6,4) exactly */
    seq_t val = binomial(n + 6, 4);
    /* Integer division truncates to the floor for positive values */
    return val / (seq_t)15;
}
