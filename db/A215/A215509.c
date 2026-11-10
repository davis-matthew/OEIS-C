/*
 * A215509  Numerator of sigma(n)/n when n belongs to A162657.
 * https://oeis.org/A215509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = numerator(sigma(m)/m) with m = A162657(n), the least m
 *            whose sigma(m)/m has denominator n; found by searching m = n*k
 *            for the smallest k with gcd(m,sigma(m)) = k, so a(n) =
 *            sigma(m)/k
 * Range:     u8 n <= 17, u16 n <= 101, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 79 terms through n = 910 (2026-11-07)
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/gcd.h"

static seq_t A215509_sigma_u64(uint64_t m)
{
    seq_t s = 1;
    uint64_t t = m;
    for (uint64_t p = 2; p * p <= t; p += (p == 2 ? 1 : 2)) {
        if (t % p == 0) {
            seq_t term = 1, sum = 1;
            while (t % p == 0) {
                t /= p;
                term = term * (seq_t)p;
                sum = sum + term;
            }
            s = s * sum;
        }
    }
    if (t > 1)
        s = s * ((seq_t)t + 1);
    return s;
}

seq_t a(idx_t n)
{
    uint64_t k = 1;
    for (;;) {
        uint64_t m = (uint64_t)n * k;
        seq_t sg = A215509_sigma_u64(m);
        seq_t g = gcd((seq_t)m, sg);
        if (g == (seq_t)k)
            return sg / (seq_t)k;
        k++;
    }
}
