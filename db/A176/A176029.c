/*
 * A176029  a(n)= n^Omega(n).
 * https://oeis.org/A176029
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = n^A001222(n) (n^Omega(n))
 * Range:     u8 n <= 7, u16 n <= 15, u32 n <= 63, u64 n <= 255,
 *            u128 n <= 4095, big any n
 * Validated: standard, pass, 63 terms through n = 8585 (2026-11-01)
 * Helpers:   bigomega, ipow_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stdint.h>
#include "../utils/bigomega.h"
#include "../utils/ipow_idx.h"

seq_t a(idx_t n)
{
    seq_t om = bigomega(n);          /* number of prime factors of n (with multiplicity) */
    return ipow_idx((seq_t)n, (idx_t)om);  /* n^om */
}
