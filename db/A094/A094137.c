/*
 * A094137  Values y of smallest positive pair (x,y) satisfying x^2 - 2*y^2 =
 *          +-d, where d=A058529(n).
 * https://oeis.org/A094137
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = y of the smallest positive pair (x,y) with x^2 - 2*y^2 =
 *            +-d, d = A058529(n); found by search over x+y, then x
 * Range:     u8 n <= 83, u16 n <= 83, u32 n <= 83, u64 n <= 83, u128 n <= 83,
 *            big any n
 * Validated: partial, pass, 83 terms through n = 83 (2026-11-07)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

static bool A094137_good(uint64_t m)
{
    if (m == 1) return true;
    for (uint64_t p = 2; p * p <= m; p++) {
        if (m % p == 0) {
            uint64_t r = p % 8;
            if (r != 1 && r != 7) return false;
            while (m % p == 0) m /= p;
        }
    }
    if (m > 1) {
        uint64_t r = m % 8;
        if (r != 1 && r != 7) return false;
    }
    return true;
}

seq_t a(idx_t n)
{
    uint64_t d = 0;
    idx_t cnt = 0;
    while (cnt < n) {
        d++;
        if (A094137_good(d)) cnt++;
    }
    long long D = (long long)d;
    for (long long s = 2;; s++) {
        for (long long x = 1; x < s; x++) {
            long long y = s - x;
            long long v = x * x - 2 * y * y;
            if (v == D || v == -D) return (seq_t)y;
        }
    }
}
