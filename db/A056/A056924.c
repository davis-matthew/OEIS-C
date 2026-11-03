/*
 * A056924  Number of divisors of n that are smaller than sqrt(n).
 * https://oeis.org/A056924
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, changed
 * Formula:   a(n) = ⌊d(n)/2⌋ , d(n) = number of positive divisors of n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 121 terms through n = 9621 (2026-11-01)
 * Helpers:   num_divisors
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/num_divisors.h"

seq_t a(idx_t n)
{
    /* idx_t is unsigned 64‑bit; safe to cast to uint64_t */
    uint64_t d = num_divisors((uint64_t)n);
    return (seq_t)(d / 2ULL);
}
