/*
 * A101300  Second-smallest prime larger than n.
 * https://oeis.org/A101300
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = second-smallest prime larger than n
 * Range:     u8 n <= 240, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 87 terms through n = 9989 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include <stddef.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n)
{
    uint64_t start = (uint64_t)n + 1;
    uint64_t p1 = start;
    while (!is_prime_u64(p1)) ++p1;
    uint64_t p2 = p1 + 1;
    while (!is_prime_u64(p2)) ++p2;
    return (seq_t)p2;
}
