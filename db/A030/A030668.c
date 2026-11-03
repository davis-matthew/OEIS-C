/*
 * A030668  Smallest nontrivial extension of n which is a cube.
 * https://oeis.org/A030668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 10, u32 n <= 430, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 51 terms through n = 9619 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Algorithm: for given n, find the smallest perfect cube that has n as a
 * leading decimal prefix and is larger than n (i.e., at least one digit is
 * appended). The search proceeds by increasing the number of appended
 * digits t = 1,2,3,… and checking the interval [n·10^t, (n+1)·10^t – 1].
 * For each t, compute the minimal cube in that interval by taking the ceil
 * of the cube root of the lower bound. The first interval that contains a
 * cube yields the answer. */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor cube root by binary search */
static inline seq_t floor_cube_root(seq_t x)
{
    seq_t lo = 0, hi = 1;
    /* raise hi until hi^3 > x */
    while (ipow_i32(hi, 3) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid3 = ipow_i32(mid, 3);
        if (mid3 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* safety; the offset is 1 */
    seq_t sn = (seq_t)n;
    seq_t pow10 = 1;                /* 10^(t-1) for the current t */

    for (;;) {
        pow10 *= 10;                /* now pow10 = 10^t , t ≥ 1 */

        seq_t low  = sn * pow10;    /* n · 10^t */
        seq_t high = low + pow10 - 1; /* (n+1)·10^t – 1 */

        seq_t root = floor_cube_root(low);      /* floor(cuberoot(low)) */
        if (root * root * root < low)           /* adjust to ceil */
            root += 1;

        seq_t cube = root * root * root;        /* minimal cube ≥ low */

        if (cube <= high)                       /* cube lies in the interval */
            return cube;                        /* first such interval gives the minimal cube */
    }
}
