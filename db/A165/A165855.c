/*
 * A165855  Totally multiplicative sequence with a(p) = 34.
 * https://oeis.org/A165855
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 34^bigomega(n)
 * Range:     u8 n <= 3, u16 n <= 15, u32 n <= 127, u64 n <= 8191,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 59 terms through n = 9597 (2026-11-01)
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
    if (n == 0) return (seq_t)0;          /* offset starts at 1, 0 is unused */
    seq_t omega = bigomega(n);             /* number of prime factors of n (with multiplicity) */
    return ipow((seq_t)34, omega);      /* 34 raised to that power */
}
