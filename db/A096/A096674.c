/*
 * A096674  Semiperimeter of the first primitive d-arithmetic triangle, where
 *          d=A072330(n).
 * https://oeis.org/A096674
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   d = A072330(n) (n-th integer whose prime factors are all 1 or 11
 *            mod 12); a(n) = 3*m where m is the least m > d with gcd(2m,d)=1
 *            and (m^2-d^2)/3 a perfect square (sides 2m-d, 2m, 2m+d;
 *            semiperimeter 3m). Found by search
 * Range:     u8 n <= 5, u16 n <= 25, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Validated: partial, pass, 50 terms through n = 50 (2026-11-07)
 * Helpers:   gcd, is_square
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/is_square.h"
#include "../utils/gcd.h"

static bool A096674_allowed(idx_t x)
{
    if (x % 2 == 0 || x % 3 == 0) return false;
    for (idx_t p = 5; p * p <= x; p += 2) {
        if (x % p == 0) {
            idx_t r = p % 12;
            if (r != 1 && r != 11) return false;
            while (x % p == 0) x /= p;
        }
    }
    if (x > 1) {
        idx_t r = x % 12;
        if (r != 1 && r != 11) return false;
    }
    return true;
}

static idx_t A096674_d(idx_t n)
{
    if (n == 1) return 1;
    idx_t cnt = 1, x = 1;
    while (cnt < n) {
        ++x;
        if (A096674_allowed(x)) ++cnt;
    }
    return x;
}

seq_t a(idx_t n)
{
    idx_t dd = A096674_d(n);
    seq_t d = (seq_t)dd;
    seq_t d2 = d * d;
    for (seq_t m = d + 1;; ++m) {
        seq_t diff = m * m - d2;
        if (diff % 3 != 0) continue;
        if (!is_square(diff / 3)) continue;
        if (gcd(m, d) != 1) continue;
        if (d % 2 == 0 && m % 2 == 0) continue;
        return 3 * m;
    }
}
