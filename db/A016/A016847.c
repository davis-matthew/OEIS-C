/*
 * A016847  a(n) = (4n+3)^11.
 * https://oeis.org/A016847
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (4n+3)^11
 * Range:     u8 none, u16 none, u32 n <= 1, u64 n <= 13, u128 n <= 795,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    seq_t base = (seq_t)4 * (seq_t)n + 3;   /* compute 4n+3 in seq_t */
    return big_pow(base, 11);             /* raise to the 11th power */
}
