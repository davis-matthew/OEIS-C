/*
 * A165856  Totally multiplicative sequence with a(p) = 35.
 * https://oeis.org/A165856
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 35^bigomega(n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9247 (2026-11-01)
 * Helpers:   bigomega, ipow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/bigomega.h"
#include "../utils/ipow.h"

seq_t a(idx_t n)
{
    seq_t omega = bigomega(n);          /* number of prime factors counting multiplicity */
    return ipow((seq_t)35, omega);   /* 35 raised to that power */
}
