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
 * Helpers:   class_number_4m, g_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/class_number_4m.h"
#include "../utils/g_u64.h"

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
