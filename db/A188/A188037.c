/*
 * A188037  a(n) = floor(n*r) - 1 - floor((n-1)*r), where r = sqrt(2).
 * https://oeis.org/A188037
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = floor(n*sqrt(2)) - floor((n-1)*sqrt(2)) - 1
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 5000, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                     /* not used, but safe for any order of calls */
    /* compute floor(n*sqrt(2)) as floor(sqrt(2*n^2)) */
    seq_t nn  = (seq_t)n;
    seq_t n1  = (seq_t)(n - 1);
    seq_t fn  = floor_sqrt(((seq_t)2 * (nn * nn)));
    seq_t fn1 = floor_sqrt(((seq_t)2 * (n1 * n1)));
    return (fn - fn1) - 1;             /* diff is 1 or 2 */
}
