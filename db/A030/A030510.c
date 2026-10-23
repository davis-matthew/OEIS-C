/*
 * A030510  Graham-Sloane-type lower bound on the size of a ternary (n,3,10)
 *          constant-weight code.
 * https://oeis.org/A030510
 *
 * Offset:    10    (a(10) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ceil( binomial(n,10) * 2^10 / (2*n + 1) )
 * Range:     u8 none, u16 n <= 11, u32 n <= 25, u64 n <= 37, u128 n <= 37,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>   /* for uint64_t */

seq_t a(idx_t n)
{
    /* Handle indices below the offset harmlessly */
    if (n < 10)
        return (seq_t)0;

    /* Compute binomial(n,10) multiplicatively using only integer ops */
    seq_t bin = (seq_t)1;
    for (idx_t i = 1; i <= 10; ++i)
        bin = bin * (seq_t)(n - i + 1) / (seq_t)i;

    /* Multiply by 2^10 = 1024 */
    seq_t term = bin * (seq_t)1024;

    /* Denominator 2*n + 1 */
    seq_t denom = (seq_t)(2 * n + 1);

    /* Ceil division: (term + denom - 1) / denom */
    seq_t result = (term + denom - (seq_t)1) / denom;

    return result;
}
