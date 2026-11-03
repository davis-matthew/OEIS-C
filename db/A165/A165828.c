/*
 * A165828  Totally multiplicative sequence with a(p) = 7.
 * https://oeis.org/A165828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, mult
 * Formula:   a(n) = 7^bigomega(n)
 * Range:     u8 n <= 7, u16 n <= 63, u32 n <= 4095, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 66 terms through n = 9468 (2026-11-01)
 * Helpers:   bigomega, ipow_u64_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/ipow_u64_idx.h"

seq_t a(idx_t n)
{
    /* bigomega(n) returns the number of prime factors of n
       (counted with multiplicity).  The result fits in idx_t
       for the values of n we are interested in. */
    idx_t omega = (idx_t)bigomega(n);
    return ipow_u64_idx(7, omega);
}
