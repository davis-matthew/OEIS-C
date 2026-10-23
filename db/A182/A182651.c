/*
 * A182651  Algorithm: x->[rx] if x even, x->[x/r] if x odd, where r =
 *          sqrt(2); a(n) is the number of steps from n to 2.
 * https://oeis.org/A182651
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of steps from n to 2 following the rule even →
 *            floor(n * sqrt(2)) odd → floor(n / sqrt(2)) with a(1) = a(2) =
 *            1. The function uses integer arithmetic only
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Base cases */
    if (n <= 2)
        return (seq_t)1;

    seq_t steps = 0;
    seq_t cur = (seq_t)n;

    while (cur != (seq_t)2)
    {
        /* cur is odd or even; decide next term */
        if ((cur & (seq_t)1) == 0)           /* even */
        {
            /* floor( sqrt( 2 * cur^2 ) ) = floor( cur * sqrt(2) ) */
            cur = floor_sqrt( (seq_t)2 * cur * cur );
        }
        else                                 /* odd */
        {
            /* floor( sqrt( cur^2 / 2 ) ) = floor( cur / sqrt(2) ) */
            cur = floor_sqrt( cur * cur / (seq_t)2 );
        }

        ++steps;
    }

    return steps;
}
