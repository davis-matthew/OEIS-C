/*
 * A225810  a(n) = (10^n)^2 + 4*(10^n) + 1.
 * https://oeis.org/A225810
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (10^n)^2 + 4*(10^n) + 1
 * Range:     u8 n <= 1, u16 n <= 2, u32 n <= 4, u64 n <= 9, u128 n <= 19,
 *            big any n
 * Helpers:   pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    seq_t t = pow10(n);      /* t = 10^n */
    seq_t res = t * t + (seq_t)4 * t + (seq_t)1;
    return res;
}
