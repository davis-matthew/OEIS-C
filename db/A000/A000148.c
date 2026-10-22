/*
 * A000148  Number of partitions into non-integral powers.
 * https://oeis.org/A000148
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x1,x2): 1 <= x1 <= x2, x1^(2/3) + x2^(2/3) <= n},
 *            counted by a two-pointer search. Exact test: with C = n^3 - x1^2
 *            - x2^2, x1^(2/3) + x2^(2/3) <= n <=> C >= 0 and C^3 >=
 *            27*n^3*x1^2*x2^2
 * Range:     u8 n <= 12, u16 n <= 76, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

typedef unsigned __int128 a148_u128;

static bool a148_ok(uint64_t n, uint64_t n3, uint64_t a, uint64_t b)
{
    a148_u128 A = (a148_u128)a * a;
    a148_u128 B = (a148_u128)b * b;
    if (A + B > (a148_u128)n3) return false;
    a148_u128 C = (a148_u128)n3 - A - B;
    a148_u128 lhs = C * C * C;
    a148_u128 rhs = (a148_u128)27 * n3 * A * B;
    (void)n;
    return lhs >= rhs;
}

seq_t a(idx_t n)
{
    uint64_t nn = (uint64_t)n;
    uint64_t n3 = nn * nn * nn;
    uint64_t b = 0;
    /* largest b with b^2 <= n3 - 1 */
    {
        uint64_t lo = 0, hi = 1;
        while (hi * hi <= n3 - 1) hi <<= 1;
        while (hi - lo > 1) {
            uint64_t mid = lo + (hi - lo) / 2;
            if (mid * mid <= n3 - 1) lo = mid; else hi = mid;
        }
        b = lo;
    }
    uint64_t cnt = 0;
    for (uint64_t x = 1; (a148_u128)8 * x * x <= (a148_u128)n3; x++) {
        while (b >= x && !a148_ok(nn, n3, x, b)) b--;
        if (b < x) break;
        cnt += b - x + 1;
    }
    return (seq_t)cnt;
}
