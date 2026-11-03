/*
 * A062174  a(n) = 3^(n-1) mod n.
 * https://oeis.org/A062174
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^(n-1) mod n
 * Range:     u8 n <= 298, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 106 terms through n = 967 (2026-11-01)
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
    /* n is guaranteed >= 1 by the OEIS offset */
    idx_t exp = n - 1;
    seq_t base = (seq_t)3;
    seq_t mod  = (seq_t)n;
    return powmod(base, exp, mod);
}
