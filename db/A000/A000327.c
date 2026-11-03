/*
 * A000327  Number of partitions into non-integral powers.
 * https://oeis.org/A000327
 *
 * Offset:    3    (a(3) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x1,x2): 1 <= x1 < x2, x1^(2/3) + x2^(2/3) <= n}.
 *            Counted with a two-pointer sweep. The test is exact in integers:
 *            with D = n^3 - x^2 - y^2, x^(2/3) + y^(2/3) <= n iff D >= 0 and
 *            D^3 >= 27*n^3*x^2*y^2
 * Range:     u8 n <= 12, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 61 terms through n = 969 (2026-11-01)
 * Helpers:   isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/isqrt_u64.h"

static bool a327_ok(uint64_t n3, uint64_t x, uint64_t y)
{
    uint64_t X = x * x + y * y;
    if (n3 < X) return false;
    unsigned __int128 D = (unsigned __int128)(n3 - X);
    unsigned __int128 lhs = D * D * D;
    unsigned __int128 rhs = (unsigned __int128)27 * n3;
    rhs *= (unsigned __int128)(x * x);
    rhs *= (unsigned __int128)(y * y);
    return lhs >= rhs;
}

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n < 2) return count;
    uint64_t nn = (uint64_t)n;
    uint64_t n3 = nn * nn * nn;
    uint64_t m = nn - 1;
    uint64_t y = isqrt_u64(m * m * m);
    if (y < 1) y = 1;
    while (a327_ok(n3, 1, y + 1)) y++;
    while (y > 1 && !a327_ok(n3, 1, y)) y--;

    for (uint64_t x = 1;; x++) {
        while (y > x && !a327_ok(n3, x, y)) y--;
        if (y <= x) break;
        count += (seq_t)(y - x);
    }
    return count;
}
