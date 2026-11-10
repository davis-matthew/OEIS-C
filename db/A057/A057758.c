/*
 * A057758  Duplicate of A057125.
 * https://oeis.org/A057758
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = n-th number m such that 3 is a square mod m (search: m
 *            not divisible by 4 or 9, every prime factor p>3 has p == +-1 mod
 *            12)
 * Range:     i8 n <= 23, i16 n <= 23, i32 n <= 23, i64 n <= 23, i128 n <= 23,
 *            big any n
 * Validated: partial, pass, 23 terms through n = 23 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

static bool A057758_ok(uint64_t m)
{
    uint64_t x = m;
    if (x % 4 == 0 || x % 9 == 0) return false;
    if (x % 2 == 0) x /= 2;
    if (x % 3 == 0) x /= 3;
    for (uint64_t p = 5; p * p <= x; p += 2) {
        if (x % p == 0) {
            uint64_t r = p % 12;
            if (r != 1 && r != 11) return false;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) {
        uint64_t r = x % 12;
        if (r != 1 && r != 11) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    idx_t count = 0;
    uint64_t cand = 0;
    while (count < n) {
        ++cand;
        if (A057758_ok(cand)) ++count;
    }
    return (seq_t)cand;
}
