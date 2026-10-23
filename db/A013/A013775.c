/*
 * A013775  a(n) = 24^(3*n + 2).
 * https://oeis.org/A013775
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 24^(3*n+2)
 * Range:     u8 none, u16 n <= 0, u32 n <= 1, u64 n <= 3, u128 n <= 8,
 *            big any n
 * Helpers:   seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    idx_t exp = n * 3 + 2;          /* exponent 3*n + 2 */
    return seq_pow((seq_t)24, exp); /* compute 24^(3*n+2) */
}
