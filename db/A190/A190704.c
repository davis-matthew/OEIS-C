/*
 * A190704  [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(3),4,2) and [ ]=floor.
 * https://oeis.org/A190704
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((4*n+2)*sqrt(3)) - 4*floor(n*sqrt(3)) - 3
 * Range:     u8 none, u16 n <= 17, u32 n <= 132, u64 n <= 132, u128 n <= 132,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*         = floor((bn+c)*r) - b*floor(n*r) - floor(c*r) with
 *           r = sqrt(3), b = 4, c = 2.  */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t n_seq = (seq_t)n;

    /* floor(n*sqrt(3)) = floor(sqrt(3*n^2)) */
    seq_t floor_n = floor_sqrt((seq_t)3 * n_seq * n_seq);

    /* floor((4n+2)*sqrt(3)) = floor(sqrt(3*(4n+2)^2)) */
    seq_t t = (seq_t)4 * n_seq + (seq_t)2;
    seq_t floor_t = floor_sqrt((seq_t)3 * t * t);

    /* subtract the fixed parts */
    return floor_t - (seq_t)4 * floor_n - (seq_t)3;
}
