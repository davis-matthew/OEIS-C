/*
 * A138099  Irregular triangle read by rows: T(n,k) = k + floor(n/2), 1 <= k
 *          <= ceiling(n/2).
 * https://oeis.org/A138099
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   a(n) = n - floor((1/4)*(floor(sqrt(4*n-3))-1)^2) where n ≥ 1
 *            (OEIS offset 1)
 * Range:     u8 n <= 77, u16 n <= 77, u32 n <= 77, u64 n <= 77, u128 n <= 77,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square‑root for unsigned __int128.
   Returns floor(sqrt(x)).  Works for any x that fits in 128 bits. */
static inline uint64_t
u128_sqrt(unsigned __int128 x)
{
    unsigned __int128 res = 0;
    unsigned __int128 bit = (unsigned __int128)1 << 126; /* highest power of 4 <= 2^128-1 */

    /* Shift bit down until it is <= x. */
    while (bit > x) bit >>= 2;

    /* Reconstruction loop. */
    while (bit != 0) {
        if (x >= res + bit) {
            x -= res + bit;
            res = (res >> 1) + bit;
        } else {
            res >>= 1;
        }
        bit >>= 2;
    }
    return (uint64_t)res;
}

/* Main sequence function. */
seq_t a(idx_t n)
{
    /* Defensive handling for an offset below 1 – not used in tests. */
    if (n == 0) return 0;

    /* Compute floor(sqrt(4*n-3)) with 128‑bit arithmetic to avoid overflow. */
    unsigned __int128 tmp = (__uint128_t)4 * n - 3;   /* 4*n-3 fits in 128 bits */
    uint64_t s = u128_sqrt(tmp);                    /* floor(sqrt(4*n-3)) */

    /* Compute the subtraction term: floor((s-1)^2 / 4). */
    uint64_t s_minus1 = s - 1;
    unsigned __int128 prod = (unsigned __int128)s_minus1 * s_minus1;
    uint64_t sub = (uint64_t)(prod / 4);   /* division is exact in integer arithmetic */

    return (seq_t)n - (seq_t)sub;
}
