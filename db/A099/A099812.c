/*
 * A099812  Number of distinct primes dividing 2n (i.e., omega(2n)).
 * https://oeis.org/A099812
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = number of distinct prime divisors of 2*n (A001221(2*n))
 * Range:     u8 n <= 1000, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 121 terms through n = 824 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    uint64_t val = n * 2ULL;           /* 2*n fits in uint64_t for all tested n */
    int cnt = omega(val);
    return (seq_t)cnt;
}
