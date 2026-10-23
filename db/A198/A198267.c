/*
 * A198267  Round(n*sqrt(11)).
 * https://oeis.org/A198267
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = round(n*sqrt(11))
 * Range:     u8 n <= 2, u16 n <= 38, u32 n <= 9694, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Implementation uses integer arithmetic only, without floating-point. */
/* We compute m = floor(n*sqrt(11)) as floor_sqrt(n*n*11).  Then we test
 * whether the fractional part is ≥ 0.5 by comparing 4*n^2*11 to (2*m+1)^2.
 * If 4*n^2*11 < (2*m+1)^2 we round down, otherwise round up. */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t for arithmetic that may overflow idx_t */
    seq_t nn = (seq_t)n;

    /* m = floor(n*sqrt(11)) = floor(sqrt(n^2*11)) */
    seq_t w = nn * nn * (seq_t)11;
    seq_t m = floor_sqrt(w);

    /* left = 4*n^2*11 = 44*n^2 */
    seq_t left = (seq_t)44 * nn * nn;

    /* right = (2*m + 1)^2 */
    seq_t twom1 = (seq_t)2 * m + (seq_t)1;
    seq_t right = twom1 * twom1;

    /* if left < right -> fractional part < 0.5 => round down */
    if (left < right)
        return m;
    else
        return m + (seq_t)1;
}
