/*
 * A229109  a(n) = n plus the number of its distinct prime factors.
 * https://oeis.org/A229109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + number of distinct prime factors of n (A001221)
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9595 (2026-11-01)
 * Helpers:   omega
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/omega.h"

seq_t a(idx_t n)
{
    /* Use the supplied helper to count distinct prime factors of n. */
    int cnt = omega((uint64_t)n);
    return (seq_t)n + (seq_t)cnt;
}
