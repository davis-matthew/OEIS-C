/*
 * A165863  Totally multiplicative sequence with a(p) = 42.
 * https://oeis.org/A165863
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 42^Ω(n) where Ω(n) is the total number of prime factors
 *            of n
 * Range:     u8 n <= 3, u16 n <= 7, u32 n <= 63, u64 n <= 4095,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 8883 (2026-11-01)
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
    if (n < 1) return 0;
    return ipow((seq_t)42, bigomega(n));
}
