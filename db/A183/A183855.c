/*
 * A183855  n+floor(sqrt(3n-3)); complement of A128422.
 * https://oeis.org/A183855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n + floor(sqrt(3n-3))
 * Range:     u8 n <= 22, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* n is guaranteed to be >= 1 for valid calls */
    seq_t nseq = (seq_t)n;
    seq_t val   = 3 * nseq - 3;     /* compute 3n-3 */
    seq_t sq    = floor_sqrt(val);  /* integer square root */
    return nseq + sq;               /* n + floor(sqrt(3n-3)) */
}
