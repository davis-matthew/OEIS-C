/*
 * A027664  a(n)/10000000 gives sqrt(n) to 7 places.
 * https://oeis.org/A027664
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = round(sqrt(n)*10000000) (OEIS A027664)
 * Range:     u8 none, u16 none, u32 none, u64 n <= 26, u128 n <= 26,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Scale factor 10^14 = (10^7)^2 for integer arithmetic */
    const seq_t SCALE = (seq_t)100000000000000ULL;  /* 10^14 */

    seq_t X = (seq_t)n * SCALE;          /* X = n * 10^14 */
    seq_t s = floor_sqrt(X);             /* s = floor(sqrt(X))  */

    /* Round to nearest integer: need to check if fractional part >= 0.5
       Equivalent to: 4*X >= (2*s + 1)^2  */
    seq_t fourX = X * 4;
    seq_t m     = s * 2 + 1;
    seq_t m2    = m * m;
    if (fourX >= m2) {
        ++s;
    }

    return s;
}
