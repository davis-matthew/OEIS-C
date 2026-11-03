/*
 * A036155  19^n mod 191.
 * https://oeis.org/A036155
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 19^n mod 191
 * Range:     u8 n <= 49, u16 n <= 49, u32 n <= 49, u64 n <= 49, u128 n <= 49,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 49 (2026-11-01)
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
    seq_t base = 19;
    seq_t mod  = 191;
    return powmod(base, n, mod);
}
