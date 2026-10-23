/*
 * A187588  Number of 5-step E, S, NW and NE-moving king's tours on an n X n
 *          board summed over all starting positions.
 * https://oeis.org/A187588
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 202*n^2 - 912*n + 994 for n > 3 (special cases: a(1) =
 *            a(2) = 0, a(3) = 106)
 * Range:     u8 n <= 3, u16 n <= 20, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n == 1 || n == 2)
        return (seq_t)0;
    if (n == 3)
        return (seq_t)106;
    /* n > 3: use the quadratic formula */
    seq_t s = (seq_t)n;
    /* compute 202*s*s - 912*s + 994 */
    seq_t term = ((seq_t)202 * s * s) - ((seq_t)912 * s) + (seq_t)994;
    return term;
}
