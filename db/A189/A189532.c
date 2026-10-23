/*
 * A189532  n+[nr/t]+[ns/t]; r=1, s=2^(1/3), t=2^(2/3).
 * https://oeis.org/A189532
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + floor(n * 2^(-2/3)) + floor(n * 2^(-1/3)) Equivalent
 *            integer form: a(n) = n + ⌊ (n^3 / 4)^(1/3) ⌋ + ⌊ (n^3 / 2)^(1/3)
 *            ⌋ where the cube roots are integer floor values. The function
 *            below computes the floor of the integer cube root of a 128‑bit
 *            integer without using floating point and re...
 * Range:     u8 n <= 84, u16 n <= 84, u32 n <= 84, u64 n <= 84, u128 n <= 84,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Return ⌊cuberoot(X)⌋ for X ≤ 2^126 – 1 (fits in 128 bits).  */
static seq_t cube_root_floor_uint128(__uint128_t X)
{
    /* Find an upper bound 2^k >= cube_root(X) */
    __uint128_t low = 0;
    __uint128_t high = 1;
    while (true) {
        __uint128_t mid2 = high * high;          /* high^2 fits in 128 bits   */
        if (mid2 == 0 || high == 0) break;       /* overflow safeguard          */
        if (mid2 > X) break;                     /* high^3 > X, stop            */
        if (mid2 > X / high) break;              /* high^3 > X, stop            */
        high <<= 1;
    }
    /* Binary search between low and high-1 (inclusive) */
    while (low < high) {
        __uint128_t mid = (low + high + 1) >> 1; /* mid = ⌈(low+high)/2⌉      */
        __uint128_t mid2 = mid * mid;            /* mid^2                       */
        if (mid2 == 0) {                         /* overflow → too large          */
            high = mid - 1;
            continue;
        }
        /* Check if mid^3 <= X by comparing mid <= X / mid^2 */
        if (mid <= X / mid2) {
            low = mid;
        } else {
            high = mid - 1;
        }
    }
    return (seq_t)low;
}

seq_t a(idx_t n)
{
    __uint128_t n128 = n;
    __uint128_t X1 = n128 * n128 * n128 / 4; // floor(n^3 / 4)
    __uint128_t X2 = n128 * n128 * n128 / 2; // floor(n^3 / 2)

    seq_t r1 = cube_root_floor_uint128(X1);
    seq_t r2 = cube_root_floor_uint128(X2);

    return (seq_t)n + r1 + r2;
}
