/*
 * A024559  a(n) = [ 1/{n*sqrt(6)} ], where {x} := x - [ x ].
 * https://oeis.org/A024559
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( 1 / { n*sqrt(6) } ) where {x} = x - floor(x)
 * Range:     u8 n <= 3, u16 n <= 52, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Convert index to seq_t */
    seq_t nn = (seq_t)n;

    /* Compute 6 * n^2 */
    seq_t six_n2 = (seq_t)6 * nn * nn;

    /* m = floor( sqrt(6 * n^2) ) = floor(n*sqrt(6)) */
    seq_t m = floor_sqrt(six_n2);

    /* d = 6*n^2 - m^2  (>= 1) */
    seq_t d = six_n2 - m * m;

    /* Upper bound for k: k <= (2*m)/d + 1
       Set high to this value (at least 1) */
    seq_t two_m = (seq_t)2 * m;
    seq_t high = two_m / d + 1;
    if (high < (seq_t)1) high = (seq_t)1;

    /* Binary search for the largest k such that
       d*k^2 <= 2*m*k + 1  */
    seq_t low = (seq_t)1;
    while (low < high) {
        seq_t mid = (low + high + 1) / (seq_t)2;

        seq_t left  = d * mid * mid;          /* d * k^2 */
        seq_t right = two_m * mid + (seq_t)1; /* 2*m*k + 1 */

        if (left <= right) {
            low = mid;          /* mid satisfies inequality, try to increase */
        } else {
            high = mid - (seq_t)1;
        }
    }
    return low;
}
