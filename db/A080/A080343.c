/*
 * A080343  a(n) = round(sqrt(2*n)) - floor(sqrt(2*n)).
 * https://oeis.org/A080343
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = round(sqrt(2*n)) - floor(sqrt(2*n))
 * Range:     u8 n <= 31, u16 n <= 8076, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t N   = (seq_t)n;        /* Convert index to seq_t */
    seq_t twoN = N + N;          /* 2*n */
    seq_t m    = floor_sqrt(twoN);  /* floor(sqrt(2*n)) */
    seq_t r    = twoN - m * m;   /* remainder 2*n - m^2   */
    /* a(n)=1 iff the fractional part of sqrt(2*n) is ≥ 0.5,
       which is equivalent to r ≥ m+1 */
    return (r >= m + (seq_t)1) ? (seq_t)1 : (seq_t)0;
}
