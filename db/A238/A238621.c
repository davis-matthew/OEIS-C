/*
 * A238621  Position of first occurrence of n in A238525.
 * https://oeis.org/A238621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = least m >= 1 with A238525(m) = n, where A238525(m) = m
 *            mod sopfr(m) and A238525(1) = 0; found by a linear search
 * Range:     u8 n <= 23, u16 n <= 845, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 84 terms through n = 940 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

static uint64_t A238621_sopfr(uint64_t m)
{
    uint64_t s = 0;
    while ((m & 1ULL) == 0) {
        s += 2;
        m >>= 1;
    }
    for (uint64_t p = 3; p <= m / p; p += 2) {
        while (m % p == 0) {
            s += p;
            m /= p;
        }
    }
    if (m > 1)
        s += m;
    return s;
}

seq_t a(idx_t n)
{
    uint64_t v = (uint64_t)n;
    if (v == 0)
        return (seq_t)1;
    for (uint64_t m = 2;; m++) {
        uint64_t s = A238621_sopfr(m);
        if (s > v && m % s == v)
            return (seq_t)m;
    }
}
