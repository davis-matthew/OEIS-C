/*
 * A188039  Positions of 0 in A188038; complement of A188040.
 * https://oeis.org/A188039
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*n - 1 + 2*floor((n-1)*sqrt(2)) (for n ≥ 1)
 * Range:     u8 n <= 44, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root of a 128‑bit unsigned value.
   Returns ⌊√x⌋. */
static uint64_t isqrt_uint128(unsigned __int128 x)
{
    unsigned __int128 low  = 0;
    unsigned __int128 high = ((unsigned __int128)1) << 61;   /* > √(2¹²¹) */
    while (low < high)
    {
        unsigned __int128 mid = low + ((high - low + 1) >> 1);
        if (mid * mid <= x)
            low = mid;
        else
            high = mid - 1;
    }
    return (uint64_t)low;
}

seq_t a(idx_t n)
{
    /* Fixed-point scaling: sqrt(2) ≈ P / 2^60, where P = ⌊sqrt(2) * 2^60⌋. */
    static const unsigned __int128 SCALE = ((unsigned __int128)1) << 60;
    static uint64_t P = 0;   /* will be computed on first call */

    if (P == 0) {
        unsigned __int128 big = ((unsigned __int128)1) << 121;   /* 2^(2*60+1) */
        P = isqrt_uint128(big);                                 /* ⌊2^60 * √2⌋ */
    }

    /* floor((n-1)*sqrt(2)) = ((n-1) * P) >> 60 */
    unsigned __int128 prod = ((unsigned __int128)(n - 1)) * P;
    seq_t floor_val = (seq_t)(prod >> 60);

    /* a(n) = 3*n - 1 + 2*floor((n-1)*sqrt(2)) */
    seq_t res = (seq_t)3 * (seq_t)n - 1 + (seq_t)2 * floor_val;
    return res;
}
