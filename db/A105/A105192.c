/*
 * A105192  Number of divisors of n101.
 * https://oeis.org/A105192
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = number of divisors of (n × 1000 + 101)
 * Range:     u8 n <= 111, u16 n <= 111, u32 n <= 111, u64 n <= 111,
 *            u128 n <= 111, big any n
 * Helpers:   count_divisors_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/count_divisors_uint64.h"

/* Sequence A105192: Number of divisors of n101.
   For any n ≥ 0, the decimal number n101 equals n × 1000 + 101. */
seq_t a(idx_t n)
{
    /* compute m = n*1000 + 101  (use 64‑bit intermediate) */
    uint64_t m = (uint64_t)n * 1000ULL + 101ULL;

    return count_divisors_uint64(m);
}
