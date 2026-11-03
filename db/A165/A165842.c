/*
 * A165842  Totally multiplicative sequence with a(p) = 21.
 * https://oeis.org/A165842
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 21^A001222(n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 255, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9344 (2026-11-01)
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
    if (n == 0) return 0;          /* undef. but safe for any call */
    seq_t omega = bigomega(n);     /* count of prime factors with multiplicity */
    return ipow((seq_t)21, omega);
}
