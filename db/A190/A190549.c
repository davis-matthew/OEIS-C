/*
 * A190549  a(n) = [(bn+c)r]-b[nr]-[cr], where (r,b,c)=(sqrt(2),4,1) and
 *          []=floor.
 * https://oeis.org/A190549
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 1, u16 n <= 22, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula for A190549:
   a(n) = floor((bn+c) * sqrt(2)) - b * floor(n * sqrt(2)) - floor(c * sqrt(2)),
   with r = sqrt(2), b = 4, c = 1.
   This reduces to
   a(n) = floor(sqrt(2*(4n+1)^2)) - 4 * floor(sqrt(2*n^2)) - 1
*/

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is at least 1 because the offset is 1 */
    seq_t t = (seq_t)4 * n + (seq_t)1;   /* bn + c = 4n + 1 */
    seq_t t2 = (seq_t)n;                /* n itself */

    seq_t p1 = floor_sqrt((seq_t)2 * t * t);   /* floor(sqrt(2*(4n+1)^2)) */
    seq_t p2 = floor_sqrt((seq_t)2 * t2 * t2); /* floor(sqrt(2*n^2)) */

    return p1 - (seq_t)4 * p2 - (seq_t)1;
}
