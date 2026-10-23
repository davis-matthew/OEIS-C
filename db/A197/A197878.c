/*
 * A197878  a(n) = floor(2*(1 + sqrt(2))*n).
 * https://oeis.org/A197878
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(2*(1 + sqrt(2))*n)
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n may be very large, compute directly using integer arithmetic:   */
    /* a(n) = 2*n + floor(2*n*sqrt(2))                                 */
    /* floor(2*n*sqrt(2)) = floor( sqrt( 2 * (2*n)^2 ) ) =             */
    /* floor_sqrt( 2 * (2*n)^2 )                                       */
    seq_t k = (seq_t)n * 2;               /* k = 2*n                                 */
    seq_t sq = k * k * 2;                 /* sq = 2*(2*n)^2                          */
    seq_t fs = floor_sqrt(sq);            /* floor(sqrt(sq)) = floor(2*n*sqrt(2))   */
    return (seq_t)n * 2 + fs;             /* combine to get a(n)                     */
}
