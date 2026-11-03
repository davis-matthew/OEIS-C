/*
 * A165844  Totally multiplicative sequence with a(p) = 23.
 * https://oeis.org/A165844
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 23^bigomega(n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9992 (2026-11-01)
 * Helpers:   bigomega, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/ipow.h"

seq_t a(idx_t n) {
    return ipow((seq_t)23, bigomega(n));
}
