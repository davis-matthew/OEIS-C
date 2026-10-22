/*
 * A006642  Class number of quadratic field with discriminant -4n+1.
 * https://oeis.org/A006642
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = h(-m), where -m is the n-th negative fundamental
 *            discriminant congruent to 1 mod 4 (m = 3, 7, 11, 15, 19, 23, 31,
 *            ... with m squarefree, m == 3 mod 4); h is computed by counting
 *            reduced forms (a,b,c) with b^2 - 4ac = -m, |b| <= a <= c, and b
 *            >= 0 if |b| = a or a = c
 * Range:     u8 n <= 68, u16 n <= 68, u32 n <= 68, u64 n <= 68, u128 n <= 68,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool sqfree_u64(uint64_t m) {
    uint64_t p;
    for (p = 2; p * p <= m; p++) {
        if (m % (p * p) == 0) return false;
        if (m % p == 0) m /= p;
    }
    return true;
}

static uint64_t class_number(uint64_t m) {
    uint64_t h = 0, b;
    for (b = 1; 3 * b * b <= m; b += 2) {
        uint64_t ac = (b * b + m) / 4;
        uint64_t x;
        for (x = b; x * x <= ac; x++) {
            if (ac % x == 0) {
                uint64_t c = ac / x;
                if (b == x || x == c) h += 1;
                else h += 2;
            }
        }
    }
    return h;
}

seq_t a(idx_t n) {
    uint64_t m = 3;
    idx_t k = 0;
    for (;; m += 4) {
        if (sqfree_u64(m)) {
            k++;
            if (k == n) break;
        }
    }
    return (seq_t)class_number(m);
}
