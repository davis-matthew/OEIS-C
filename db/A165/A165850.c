/*
 * A165850  Totally multiplicative sequence with a(p) = 29.
 * https://oeis.org/A165850
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 29^Ω(n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 61 terms through n = 9916 (2026-11-01)
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
    seq_t omega = bigomega(n);
    return ipow((seq_t)29, omega);
}
