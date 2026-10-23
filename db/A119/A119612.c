/*
 * A119612  Number of integers of the form (nk)^3/(n^3+k^3), k>=1.
 * https://oeis.org/A119612
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   For fixed n ≥ 1, a(n) is the number of positive integers k such
 *            that (n*k)^3 / (n^3 + k^3) is an integer. This is equivalent to
 *            letting n = d·a, k = d·b with (a,b)=1 and a^3 + b^3 | d^3. The
 *            function counts all such k
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   u64gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/u64gcd.h"

/* compute greatest common divisor of two idx_t (uint64_t) values */

/* integer cube root: largest r such that r^3 ≤ x */
static inline idx_t int_cuberoot(idx_t x)
{
    idx_t lo = 0, hi = 1;
    /* find an upper bound */
    while (hi * hi * hi <= x && hi < 1ULL << 22) hi <<= 1;  /* hi stays < 2^22 ≈ 4.2M */
    /* binary search */
    while (lo < hi) {
        idx_t mid = (lo + hi + 1) >> 1;
        if (mid * mid * mid <= x) lo = mid;
        else hi = mid - 1;
    }
    return lo;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;  /* offset starts at 1, but guard for safety */

    seq_t count = 0;

    /* Enumerate all divisors d of n */
    idx_t d, d2;
    for (d = 1; d * d <= n; ++d) {
        if (n % d != 0) continue;

        /* process divisor d */
        d2 = d;
        idx_t a_val = n / d2;
        uint64_t d3 = (uint64_t)d2 * d2 * d2;
        uint64_t a3 = (uint64_t)a_val * a_val * a_val;
        uint64_t rem = (d3 > a3) ? (d3 - a3) : 0;
        idx_t limit = int_cuberoot(rem);
        if (limit > 0) {
            uint64_t a3_val = a3;   /* keep a3 in 64 bits */
            for (idx_t b = 1; b <= limit; ++b) {
                if (u64gcd(a_val, b) != 1) continue;
                uint64_t b3 = (uint64_t)b * b * b;
                uint64_t m = a3_val + b3;   /* m ≤ d^3 by construction */
                if (d3 % m == 0) {
                    count++;
                }
            }
        }

        /* process paired divisor n/d if distinct */
        d2 = n / d;
        if (d2 != d) {
            idx_t a_val2 = n / d2;
            uint64_t d3 = (uint64_t)d2 * d2 * d2;
            uint64_t a3 = (uint64_t)a_val2 * a_val2 * a_val2;
            uint64_t rem = (d3 > a3) ? (d3 - a3) : 0;
            idx_t limit = int_cuberoot(rem);
            if (limit > 0) {
                uint64_t a3_val = a3;
                for (idx_t b = 1; b <= limit; ++b) {
                    if (u64gcd(a_val2, b) != 1) continue;
                    uint64_t b3 = (uint64_t)b * b * b;
                    uint64_t m = a3_val + b3;
                    if (d3 % m == 0) {
                        count++;
                    }
                }
            }
        }
    }

    return count;
}
