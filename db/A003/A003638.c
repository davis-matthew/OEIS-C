/*
 * A003638  Number of classes per genus in quadratic field with discriminant
 *          -4n, -n == 2,3( mod 4).
 * https://oeis.org/A003638
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   let m(n) be the n-th squarefree m with m == 1,2 (mod 4) (so -m
 *            == 3,2 mod 4). a(n) = h(-4m) / 2^r, where h(-4m) is the class
 *            number (counted by enumerating reduced primitive forms of
 *            discriminant -4m) and r = number of odd primes dividing m (2^r =
 *            number of genera)
 * Range:     u8 n <= 75, u16 n <= 75, u32 n <= 75, u64 n <= 75, u128 n <= 75,
 *            big any n
 * Validated: partial, pass, 75 terms through n = 75 (2026-11-01)
 * Helpers:   gcd_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/gcd_u64.h"

static uint64_t classno(uint64_t m) {
    uint64_t D = 4 * m, h = 0;
    for (uint64_t a = 1; 3 * a * a <= D; a++) {
        for (int64_t b = -(int64_t)a; b <= (int64_t)a; b++) {
            if (b & 1) continue;
            uint64_t ab = (uint64_t)(b < 0 ? -b : b);
            uint64_t num = ab * ab + D;
            if (num % (4 * a)) continue;
            uint64_t c = num / (4 * a);
            if (c < a) continue;
            if (b < 0 && (ab == a || a == c)) continue;
            if (gcd_u64(gcd_u64(a, ab), c) != 1) continue;
            h++;
        }
    }
    return h;
}

static int sqfree_ok(uint64_t m) {
    uint64_t r = m % 4;
    if (r != 1 && r != 2) return 0;
    for (uint64_t p = 2; p * p <= m; p++)
        if (m % (p * p) == 0) return 0;
    return 1;
}

seq_t a(idx_t n) {
    uint64_t m = 0, cnt = 0;
    while (cnt < n) {
        m++;
        if (sqfree_ok(m)) cnt++;
    }
    uint64_t h = classno(m);
    uint64_t t = m;
    while (t % 2 == 0) t /= 2;
    for (uint64_t p = 3; p * p <= t; p += 2) {
        if (t % p == 0) {
            h /= 2;
            while (t % p == 0) t /= p;
        }
    }
    if (t > 1) h /= 2;
    return (seq_t)h;
}
