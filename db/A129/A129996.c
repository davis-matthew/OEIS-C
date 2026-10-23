/*
 * A129996  a(n) = (-1)^[(n+1)/2] A000108([n/2]+1).
 * https://oeis.org/A129996
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign
 * Formula:   a(n) = (-1)^[(n+1)/2] * Catalan(floor(n/2)+1)
 * Range:     i8 n <= 7, i16 n <= 15, i32 n <= 31, i64 n <= 35, i128 n <= 35,
 *            big any n
 * Helpers:   catalan
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/catalan.h"

seq_t a(idx_t n)
{
    /* compute Catalan number at index k = floor(n/2) + 1 */
    idx_t k = n / 2 + 1;
    seq_t val = catalan(k);

    /* sign: negative iff floor((n+1)/2) is odd.
       Parity of floor((n+1)/2) equals ((n>>1)&1) ^ (n&1). */
    int parity = ((n >> 1) & 1) ^ (n & 1);
    if (parity)
        val = -val;

    return val;
}
