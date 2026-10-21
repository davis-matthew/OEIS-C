/*
 * A227790  Difference between 3*n^2 and the nearest square number.
 * https://oeis.org/A227790
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = min(3*n^2 - floor(n*sqrt(3))^2, (1+floor(n*sqrt(3)))^2 -
 *            3*n^2)
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n) {
    if (n == 0) return (seq_t)0;             /* offset 1, but keep safe for 0 */
    seq_t nt = (seq_t)n;                     /* cast index to seq_t first */
    seq_t sq = nt * nt;                      /* n^2 */
    sq = sq * 3;                             /* 3*n^2 */

    seq_t k   = floor_sqrt(sq);              /* floor(sqrt(3*n^2)) = floor(n*sqrt(3)) */
    seq_t k2  = k * k;                       /* k^2 */
    seq_t diff1 = sq - k2;                   /* 3*n^2 - k^2 */
    seq_t k1   = k + 1;
    seq_t diff2 = k1 * k1 - sq;              /* (k+1)^2 - 3*n^2 */

    return (diff1 <= diff2) ? diff1 : diff2;
}
