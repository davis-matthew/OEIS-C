/*
 * A204164  Symmetric matrix based on f(i,j) = floor((i+j)/2), by
 *          antidiagonals.
 * https://oeis.org/A204164
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = ceil((sqrt(8*n+1)-1)/4) implemented via the equivalent
 *            recurrence: let m = floor(sqrt(n/2)) a(n) = m if n <= m*(2*m+1)
 *            = m + 1 otherwise This approach uses only integer arithmetic and
 *            the provided floor_sqrt helper, avoiding any floating‑point
 *            computation
 * Range:     u8 n <= 3081, u16 n <= 9870, u32 n <= 9870, u64 n <= 9870,
 *            u128 n <= 9870, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;          /* offset is 1; n=0 is undefined but handled */

    /* compute m = floor(sqrt(floor(n/2))) */
    seq_t half = (seq_t)(n / 2);
    seq_t m    = floor_sqrt(half);

    /* compute t = m * (2*m + 1) */
    seq_t t = m * (((seq_t)2 * m) + 1);

    /* decide whether we need to add 1 */
    if ((seq_t)n > t)
        return m + 1;
    return m;
}
