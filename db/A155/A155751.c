/*
 * A155751  A variation on 10^n mod 17.
 * https://oeis.org/A155751
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = ((10^n mod 17)+8)%17 - 8 (signed representation of 10^n
 *            mod 17)
 * Range:     i8 n <= 79, i16 n <= 79, i32 n <= 79, i64 n <= 79, i128 n <= 79,
 *            big any n
 * Validated: partial, pass, 80 terms through n = 79 (2026-11-01)
 * Helpers:   powmod
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <string.h>
#include <stdbool.h>
#include "../utils/powmod.h"

seq_t a(idx_t n)
{
    seq_t r   = powmod(10, n, 17);       /* r = 10^n mod 17, 0 <= r <= 16 */
    seq_t tmp = (r + 8) % 17;             /* shift to 0..16 range suitable for signed mapping */
    seq_t result = tmp - 8;               /* map to -8..8 */
    return result;
}
