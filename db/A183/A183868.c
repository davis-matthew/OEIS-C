/*
 * A183868  a(n) = n + floor(2*sqrt(n+1)); complement of A079524.
 * https://oeis.org/A183868
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(2*sqrt(n+1))
 * Range:     u8 n <= 59, u16 n <= 59, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* The OEIS offset is 1; for n < 1 the result is not defined. */
    if (n < 1u) return (seq_t)0;

    seq_t n_se = (seq_t)n;
    seq_t m     = n_se + 1;          /* n + 1 */
    seq_t s     = floor_sqrt(m);     /* floor(sqrt(n+1)) */

    /* term = n + 2*floor(sqrt(n+1)) or +1 more if the fraction part ≥ 0.5 */
    seq_t term = n_se + 2 * s;

    seq_t s_sqr = s * s;
    seq_t upper = s_sqr + s;         /* s^2 + s  (threshold for +1) */

    if (m > upper)
        term += 1;

    return term;
}
