/*
 * A182058  Number of moves needed to solve the Towers of Hanoi puzzle with 6
 *          pegs and n disks.
 * https://oeis.org/A182058
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = (n - (m^4 - 2*m^3 + 11*m^2 - 34*m + 48)/24) * 2^(m-1) +
 *            1, where m = ceil((sqrt(5 + 4*sqrt(24*n + 25)) - 1) / 2) - 1
 * Range:     u8 n <= 1, u16 n <= 494, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 75 terms through n = 9560 (2026-11-01)
 * Helpers:   ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    /* X = 24*n + 25 */
    seq_t X = (seq_t)24 * (seq_t)n + (seq_t)25;

    /* s = floor(sqrt(X)), check if X is a perfect square */
    seq_t s = floor_sqrt(X);
    bool isXPerfect = (s * s == X);

    /* Y_min = 5 + 4*s, floor sqrt of Y_min */
    seq_t Y_min = (seq_t)5 + (seq_t)4 * s;
    seq_t y_floor = floor_sqrt(Y_min);
    bool isYPerfect = (y_floor * y_floor == Y_min);

    /* t = ceil(sqrt(5 + 4*sqrt(X))) */
    seq_t t = isXPerfect && isYPerfect ? y_floor : y_floor + (seq_t)1;

    /* m = t/2 - 1 */
    seq_t m = t / (seq_t)2 - (seq_t)1;

    /* pow2k = 2^(m-1) */
    idx_t pow_exp = (idx_t)(m - (seq_t)1);
    seq_t pow2k = pow2(pow_exp);

    /* thigh = (m^4 - 2*m^3 + 11*m^2 - 34*m + 48)/24 */
    seq_t m2 = m * m;
    seq_t m3 = m2 * m;
    seq_t m4 = m3 * m;
    seq_t num = m4 - (seq_t)2 * m3 + (seq_t)11 * m2 - (seq_t)34 * m + (seq_t)48;
    seq_t thigh = num / (seq_t)24;

    /* a(n) = (n - thigh) * 2^(m-1) + 1 */
    seq_t base = (seq_t)n - thigh;
    seq_t result = base * pow2k + (seq_t)1;

    return result;
}
