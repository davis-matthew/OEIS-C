/*
 * A165829  Totally multiplicative sequence with a(p) = 8.
 * https://oeis.org/A165829
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 8^bigomega(n)
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 2047, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9752 (2026-11-01)
 * Helpers:   bigomega, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    return ipow((seq_t)8, bigomega(n));
}
