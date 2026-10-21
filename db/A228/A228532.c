/*
 * A228532  a(n) = order of the torsion subgroup of the elliptic curve y^2 =
 *          x^3 + n if n is a sixth-power-free integer, otherwise -1.
 * https://oeis.org/A228532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, sign
 * Formula:   a(n) = order of the torsion subgroup of the elliptic curve y^2 =
 *            x^3 + n (zero is returned for the trivial torsion). If n has a
 *            sixth power factor a(n) = -1. For sixth‑power‑free n the torsion
 *            order is: - 1 (trivial) → 0 returned - 2 if n is a perfect cube
 *            - 3 if n is a perfect square - 6 if n is...
 * Range:     i8 n <= 87, i16 n <= 87, i32 n <= 87, i64 n <= 87, i128 n <= 87,
 *            big any n
 * Helpers:   floor_cuberoot_uint64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include <math.h>
#include "../utils/floor_cuberoot_uint64.h"

/* Integer square root for uint64_t using binary search */
static uint64_t floor_sqrt_uint64(uint64_t x)
{
    uint64_t low = 0, high = (uint64_t)1 << 32; /* sqrt(2^64-1) < 2^32 */
    while (low <= high) {
        uint64_t mid = (low + high) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq == x) return mid;
        if (sq < x)
            low = mid + 1;
        else
            high = mid - 1;
    }
    return high;
}

seq_t a(idx_t n)
{
    /* Assume n >= 1 (offset 1) */
    uint64_t val = (uint64_t)n;

    /* Check sixth‑power‑free: no prime factor with exponent >= 6 */
    uint64_t temp = val;
    bool ok = true;
    for (uint64_t p = 2; p * p <= temp; ++p) {
        int cnt = 0;
        while (temp % p == 0) {
            temp /= p;
            ++cnt;
            if (cnt >= 6) {
                ok = false;
                break;
            }
        }
        if (!ok) break;
    }
    if (!ok) return (seq_t)-1;

    /* Square and cube tests */
    bool is_sq = false, is_cb = false;
    uint64_t sq = floor_sqrt_uint64(val);
    if (sq * sq == val) is_sq = true;
    uint64_t cb = floor_cuberoot_uint64(val);
    if (cb * cb * cb == val) is_cb = true;

    /* Determine torsion order */
    unsigned ord;
    if (is_sq && is_cb)
        ord = 6;
    else if (is_sq)
        ord = 3;
    else if (is_cb)
        ord = 2;
    else
        ord = 1;

    return (ord == 1) ? (seq_t)0 : (seq_t)ord;
}
