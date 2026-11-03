/*
 * A000198  Largest order of automorphism group of a tournament with n nodes.
 * https://oeis.org/A000198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice
 * Formula:   not stated
 * Range:     u8 n <= 13, u16 n <= 23, u32 n <= 44, u64 n <= 83, u128 n <= 164,
 *            big any n
 * Validated: standard, pass, 54 terms through n = 948 (2026-11-01)
 * Helpers:   pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Formula (Alspach & Berggren 1973, Theorem 5):
   a(3r) = 3^r * a(r); a(n) = a(n-1) for n == 1, 2, 4 (mod 9);
   a(9k+7) = a(7)*a(9k); a(9k+8) = max(a(9k+7), a(5)*a(9k+3));
   a(9k+5) = max(a(2)*a(9k+3), a(5)*a(9k), a(7)*a(9k-2)) for k >= 1;
   initial values a(0) = 1, a(1) = 1, a(2) = 1, a(5) = 5, a(7) = 21. */
#include <stdint.h>
#include "../utils/pow3.h"

static seq_t a198_rec(idx_t n)
{
    if (n <= 2) return 1;
    if (n == 5) return 5;
    if (n == 7) return 21;
    if (n % 3 == 0) {
        idx_t r = n / 3;
        seq_t p = pow3(r);
        seq_t q = a198_rec(r);
        return p * q;
    }
    idx_t m = n % 9;
    idx_t k = n / 9;
    if (m == 1 || m == 2 || m == 4) return a198_rec(n - 1);
    if (m == 7) {
        seq_t q = a198_rec(9 * k);
        return q * 21;
    }
    if (m == 8) {
        seq_t x = a198_rec(9 * k + 7);
        seq_t y = a198_rec(9 * k + 3);
        y *= 5;
        return (x > y) ? x : y;
    }
    /* m == 5, k >= 1 */
    {
        seq_t best = a198_rec(9 * k + 3);
        seq_t y = a198_rec(9 * k);
        y *= 5;
        if (y > best) best = y;
        seq_t z = a198_rec(9 * k - 2);
        z *= 21;
        if (z > best) best = z;
        return best;
    }
}

seq_t a(idx_t n)
{
    return a198_rec(n);
}
