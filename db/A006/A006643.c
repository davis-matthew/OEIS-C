/*
 * A006643  Class number of quadratic field with discriminant -4n as n runs
 *          through A089269: squarefree numbers congruent to 1 or 2 mod 4.
 * https://oeis.org/A006643
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(-4*m) where m = A089269(n); h is computed by counting
 *            reduced primitive forms (a,b,c), b even, b^2-4ac = -4m, |b| <= a
 *            <= c, b >= 0 if |b| = a or a = c
 * Range:     u8 n <= 8547, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 107 terms through n = 9512 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/gcd_u64.h"

static uint64_t class_number_4m(uint64_t m) {
    /* forms a x^2 + b x y + c y^2 with b^2 - 4ac = -4m */
    uint64_t D = 4 * m;
    uint64_t h = 0;
    for (uint64_t b = 0; 3 * b * b <= D; b += 2) {
        uint64_t N = (b * b + D) / 4; /* = a*c */
        uint64_t a0 = b ? b : 1;
        for (uint64_t a = a0; a * a <= N; a++) {
            if (N % a) continue;
            uint64_t c = N / a;
            if (gcd_u64(gcd_u64(a, b), c) != 1) continue;
            if (b == 0 || a == b || a == c) h += 1;
            else h += 2;
        }
    }
    return h;
}

static bool sqfree_u64(uint64_t n) {
    uint64_t t = n;
    for (uint64_t p = 2; p * p <= t; p++) {
        if (t % p == 0) {
            t /= p;
            if (t % p == 0) return false;
        }
    }
    return true;
}

static uint64_t nth_m(idx_t n) {
    idx_t count = 0;
    uint64_t m = 0;
    while (count < n) {
        m++;
        uint64_t r = m % 4;
        if ((r == 1 || r == 2) && sqfree_u64(m)) count++;
    }
    return m;
}

seq_t a(idx_t n) {
    uint64_t m = nth_m(n);
    return (seq_t)class_number_4m(m);
}
