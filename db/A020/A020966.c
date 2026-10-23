/*
 * A020966  a(n) = Sum_{k>=1} floor(n*sqrt(2)^(2-k)).
 * https://oeis.org/A020966
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = Sum_{k>=1} floor(n*sqrt(2)^(2-k))
 * Range:     u8 n <= 5, u16 n <= 62, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return 0;

    seq_t nn   = (seq_t)n;               /* cast index to output type   */
    seq_t sum  = floor_sqrt((seq_t)2 * nn * nn);   /* floor(n*sqrt(2))       */

    seq_t cur = nn * nn;                /* current power of n^2        */
    for (;;) {
        seq_t t = floor_sqrt(cur);      /* floor( sqrt( cur ) ) = floor( n / sqrt(2)^t ) */
        if (t == 0) break;              /* rest are zero               */
        sum += t;
        cur = cur / (seq_t)2;           /* divide by 2 for next t      */
    }
    return sum;
}
