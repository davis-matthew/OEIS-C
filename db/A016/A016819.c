/*
 * A016819  a(n) = (4n+1)^7.
 * https://oeis.org/A016819
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4*n + 1)^7
 * Range:     u8 n <= 0, u16 n <= 0, u32 n <= 5, u64 n <= 20, u128 n <= 20,
 *            big any n
 * Helpers:   pow_uint
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/pow_uint.h"

seq_t a(idx_t n)
{
    /* compute the base 4*n + 1 */
    uint64_t base = (uint64_t)4 * (uint64_t)n + 1;
    return pow_uint(base, 7);
}
