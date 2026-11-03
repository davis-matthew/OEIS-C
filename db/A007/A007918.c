/*
 * A007918  Least prime >= n (version 1 of the "next prime" function).
 * https://oeis.org/A007918
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = least prime >= n; found by searching upward from max(n,2)
 *            with trial division
 * Range:     u8 n <= 251, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 90 terms through n = 9666 (2026-11-03)
 * Helpers:   is_prime_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/is_prime_u64.h"

seq_t a(idx_t n) {
    uint64_t c = (uint64_t)n;
    if (c < 2) c = 2;
    while (!is_prime_u64(c)) c++;
    return (seq_t)c;
}
