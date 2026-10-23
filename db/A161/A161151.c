/*
 * A161151  a(n) = (largest odd divisor of (n+1))*(largest power of 2 dividing
 *          n).
 * https://oeis.org/A161151
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (largest odd divisor of (n+1)) * (largest power of 2
 *            dividing n)
 * Range:     u8 n <= 15, u16 n <= 255, u32 n <= 9905, u64 n <= 9905,
 *            u128 n <= 9905, big any n
 * Helpers:   odd_part
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/odd_part.h"

seq_t a(idx_t n)
{
    /* convert index to seq_t */
    seq_t sn = (seq_t)n;

    /* largest odd divisor of (n+1) */
    seq_t odd = odd_part(sn + 1);

    /* largest power of 2 dividing n */
    idx_t t = n;
    seq_t pow2 = 1;
    while (t && ((t & 1) == 0))
    {
        pow2 <<= 1;
        t >>= 1;
    }

    return odd * pow2;
}
