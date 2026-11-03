/*
 * A161397  a(n) = the smallest positive integer that contains the same number
 *          of (non-leading) 0's as n when a(n) and n are written in binary,
 *          is not coprime to n, and is not a divisor of n.
 * https://oeis.org/A161397
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  base, nonn
 * Formula:   a(n) = smallest positive integer m such that * zero_count(m) =
 *            zero_count(n), * gcd(m,n) > 1, and n % m != 0 * (zero_count
 *            counts the number of 0 bits in the binary representation * of
 *            the number, excluding the leading 1)
 * Range:     u8 n <= 28, u16 n <= 190, u32 n <= 190, u64 n <= 2038,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9394 (2026-11-01)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd.h"

static int zeros_seq(seq_t x) {
    int bits = 0;
    seq_t y = x;
    while (y) { y >>= 1; ++bits; }
    if (bits <= 1) return 0;
    int zero = 0;
    seq_t mask = (seq_t)1;
    for (int i = 0; i < bits - 1; ++i) {
        if ((x & mask) == 0) ++zero;
        mask <<= 1;
    }
    return zero;
}

static int zeros_uint64(uint64_t x) {
    int bits = 0;
    uint64_t y = x;
    while (y) { y >>= 1; ++bits; }
    if (bits <= 1) return 0;
    int zero = 0;
    uint64_t mask = 1;
    for (int i = 0; i < bits - 1; ++i) {
        if ((x & mask) == 0) ++zero;
        mask <<= 1;
    }
    return zero;
}

seq_t a(idx_t n) {
    if (n < 2) return (seq_t)0; /* below offset, undefined */

    int zeros_n = zeros_uint64((uint64_t)n);
    seq_t m = (seq_t)1;

    for (;;) {
        if (zeros_seq(m) == zeros_n) {
            seq_t g = gcd(m, (seq_t)n);
            if (g > 1) {
                seq_t rem = (m <= (seq_t)n) ? ((seq_t)n % m) : ((seq_t)n);
                if (rem != 0)
                    return m;
            }
        }
        ++m;
    }
}
