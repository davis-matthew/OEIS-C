/*
 * A129757  a(n) = ceiling((2^n + 1 - 2*floor(2^(n/2)))/2).
 * https://oeis.org/A129757
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, less
 * Formula:   a(n) = ceil((2^n + 1 - 2*floor(2^(n/2)))/2)
 * Range:     u8 n <= 5, u16 n <= 13, u32 n <= 29, u64 n <= 32, u128 n <= 32,
 *            big any n
 * Helpers:   floor_sqrt, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* 2^n */
    seq_t pow2_n = pow2(n);

    /* floor(2^(n/2)) = floor(sqrt(2^n)) */
    seq_t fsqrt = floor_sqrt(pow2_n);

    /* numerator = 2^n + 1 - 2*floor_sqrt */
    seq_t numerator = pow2_n + 1;
    numerator -= (fsqrt << 1);

    /* ceil division by 2: (numerator + 1) >> 1 */
    return (numerator + 1) >> 1;
}
