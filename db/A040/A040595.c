/*
 * A040595  Continued fraction for sqrt(620).
 * https://oeis.org/A040595
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, cofr, easy, mult, less
 * Formula:   a(0)=24; a(2)=8; a(2^e)=48 for e>=2; a(p^e)=1 for odd prime p
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Helpers:   v2_exponent
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/v2_exponent.h"

seq_t a(idx_t n)
{
    if (n == 0)                     return (seq_t)24;
    int e = v2_exponent(n);
    if (e == 0)                     return (seq_t)1;
    if (e == 1)                     return (seq_t)8;
    /* e >= 2 */
    return (seq_t)48;
}
