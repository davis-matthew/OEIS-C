/*
 * A051726  Numerator of n*(n-1)*(n-2)/720.
 * https://oeis.org/A051726
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, frac
 * Formula:   a(n) = numerator of n*(n-1)*(n-2)/720
 * Range:     u8 n <= 9, u16 n <= 78, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Helpers:   gcd128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd128.h"

seq_t a(idx_t n)
{
    if (n < 3)                 /* product contains a zero factor  */
        return 0;

    unsigned __int128 N = (unsigned __int128)n * (n - 1) * (n - 2);

    seq_t g = gcd128(N, (unsigned __int128)720);
    return (seq_t)(N / g);
}
