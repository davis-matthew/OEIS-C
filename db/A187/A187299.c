/*
 * A187299  Number of 4-step one space leftwards or up, two space rightwards
 *          or down asymmetric rook's tours on an n X n board summed over all
 *          starting positions.
 * https://oeis.org/A187299
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 0 for n = 1,2 a(n) = 54 for n = 3 a(n) = 196 for n = 4
 *            a(n) = 480 for n = 5 a(n) = 58*n^2 - 232*n + 180 for n > 5
 *            (derived from the OEIS entry)
 * Range:     u8 n <= 4, u16 n <= 35, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n)
{
    /* Small values for n ≤ 5 */
    static const seq_t base[5] = {0, 0, 54, 196, 480};

    if (n <= 5)
        return base[(size_t)n - 1];

    /* Compute the quadratic formula using larger intermediate
       to avoid overflow for smaller seq_t types. */
    unsigned long long nn   = n;
    unsigned long long val  = 58ULL * nn * nn - 232ULL * nn + 180ULL;
    return (seq_t)val;
}
