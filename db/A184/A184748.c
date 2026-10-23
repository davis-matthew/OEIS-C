/*
 * A184748  a(n) = floor(n*r + h), where r = 4 - sqrt(5), h = -1/2. Also,
 *          complement of A184749.
 * https://oeis.org/A184748
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*(4 - sqrt(5)) - 1/2)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert the index to seq_t for precise arithmetic. */
    seq_t nsq   = (seq_t)n * (seq_t)n;          /* n^2 */
    seq_t f     = floor_sqrt(nsq * 5);          /* floor(n*sqrt(5)) */

    /* Base value of the expression before handling the ½‑offset. */
    seq_t base = (seq_t)4 * (seq_t)n - f - 1;   /* 4n - floor(n√5) - 1 */

    /*
     * Determine whether the fractional part of n*sqrt(5) is at least ½.
     * Using the inequality:
     *   20*n^2  >=  (2*floor(n√5)+1)^2     ⇔     frac(n√5) ≥ ½
     */
    seq_t lhs = (seq_t)20 * nsq;
    seq_t rhs = (seq_t)((seq_t)2 * f + 1);
    rhs = rhs * rhs;

    if (lhs >= rhs)
        base -= 1;   /* subtract an extra 1 when fraction ≥ ½ */

    return base;
}
