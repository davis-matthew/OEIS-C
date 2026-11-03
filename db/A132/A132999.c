/*
 * A132999  Imperfect numbers: Not equal to sum of proper divisors.
 * https://oeis.org/A132999
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 253, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 83 terms through n = 9436 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Imperfect numbers: a(n) is the nth positive integer that is not equal to
   the sum of its proper divisors. The function iterates over integers
   checking perfectness. */
#include <stdint.h>
#include <stdbool.h>

/* Helper: sum of proper divisors for a 64‑bit integer.  Works for the
   small numbers encountered (up to about 10 000). */
static uint64_t sum_proper_divisors_uint64(uint64_t n) {
    if (n <= 1) return 0;
    uint64_t sum = 1;
    for (uint64_t d = 2; d * d <= n; ++d) {
        if (n % d == 0) {
            uint64_t q = n / d;
            if (d == q) sum += d;
            else sum += d + q;
        }
    }
    return sum;
}

seq_t a(idx_t n) {
    idx_t imperfect_count = 0;
    uint64_t candidate = 0;
    while (imperfect_count < n) {
        ++candidate;
        uint64_t sum = sum_proper_divisors_uint64(candidate);
        if (sum != candidate) {
            ++imperfect_count;
            if (imperfect_count == n) return (seq_t)candidate;
        }
    }
    /* Unreachable but satisfies the compiler */
    return (seq_t)candidate;
}
