/*
 * A096672  Smallest side of the first primitive d-arithmetic triangle, where
 *          d=A072330(n).
 * https://oeis.org/A096672
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2m-d, where d = A072330(n) and m is the least integer > d
 *            with gcd(m,d)=1 and 3*(m^2-d^2) a perfect square (search)
 * Range:     u8 n <= 1, u16 n <= 13, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Validated: partial, pass, 54 terms through n = 54 (2026-11-07)
 * Helpers:   gcd, is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_square.h"
#include "../utils/gcd.h"

/* n-th positive integer whose prime factors are all 1 or 11 mod 12 */
static unsigned long long A072330_d(idx_t n)
{
    unsigned long long x = 1;
    idx_t cnt = 1;
    while (cnt < n) {
        ++x;
        unsigned long long y = x;
        bool ok = true;
        for (unsigned long long p = 2; p * p <= y; ++p) {
            if (y % p == 0) {
                unsigned long long r = p % 12;
                if (r != 1 && r != 11) { ok = false; break; }
                while (y % p == 0) y /= p;
            }
        }
        if (ok && y > 1) {
            unsigned long long r = y % 12;
            if (r != 1 && r != 11) ok = false;
        }
        if (ok) ++cnt;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t d = (seq_t)A072330_d(n);
    if (d == 1) return (seq_t)3;
    seq_t m = d + 1;
    for (;; ++m) {
        if (gcd(m, d) != 1) continue;
        seq_t t = (seq_t)3 * (m * m - d * d);
        if (is_square(t)) return (seq_t)2 * m - d;
    }
}
