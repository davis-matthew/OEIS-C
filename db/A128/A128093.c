/*
 * A128093  a(n) = smallest multiple of n which is >= 2^n.
 * https://oeis.org/A128093
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n * ceil(2^n / n)
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 31, u64 n <= 63, u128 n <= 127,
 *            big any n
 * Helpers:   pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;
    seq_t p  = pow2(n);                 /* 2^n as seq_t */
    seq_t ceil_div = (p + nn - 1) / nn; /* ceil(p / nn) */
    return nn * ceil_div;
}
