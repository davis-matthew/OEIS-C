/*
 * A008441  Number of ordered ways of writing n as the sum of 2 triangular
 *          numbers.
 * https://oeis.org/A008441
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = #{(i,j) | i>=0, j>=0, i(i+1)/2 + j(j+1)/2 == n} (ordered
 *            ways to express n as the sum of two triangular numbers)
 * Range:     u8 n <= 7, u16 n <= 1614, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

/* Helper to test if a value is triangular */
static bool is_triangular(seq_t x)
{
    seq_t y = 8 * x + 1;               /* 8*Triangular + 1 = odd square */
    seq_t s = floor_sqrt(y);           /* integer square root of y */
    return s * s == y;                 /* perfect square test */
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    seq_t rn = (seq_t)n; /* cast once for efficiency */

    for (idx_t i = 0; ; ++i) {
        seq_t Ti = (seq_t)i * ((seq_t)i + 1) / 2;   /* T_i = i(i+1)/2 */
        if (Ti > rn) break;                       /* beyond target */
        seq_t r = rn - Ti;                        /* remaining to reach n */
        if (is_triangular(r))
            count += 1;                           /* ordered pair found */
    }
    return count;
}
