/*
 * A061023  Difference between the closest square and the closest cube to n.
 * https://oeis.org/A061023
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn, look
 * Formula:   a(n) = |closest perfect square to n – closest perfect cube to n|
 * Range:     u8 n <= 1962, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_isqrt.h"

static idx_t cube_root_floor(idx_t n) {
    idx_t low = 0, high = (1ULL << 22);          /* (2^22)^3 = 2^66 > 2^64-1 */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;
        __uint128_t cb = (__uint128_t)mid * mid * mid;
        if (cb <= n)
            low = mid;
        else
            high = mid - 1;
    }
    return low;
}

seq_t a(idx_t n) {
    /* ---- nearest perfect square ---- */
    idx_t m = floor_isqrt(n);
    idx_t m_next = m + 1;
    __uint128_t square = (__uint128_t)m * m;
    __uint128_t nextsquare = (__uint128_t)m_next * m_next;
    __uint128_t ndiff0 = (square <= n) ? n - square : square - n;
    __uint128_t ndiff1 = (nextsquare >= n) ? nextsquare - n : n - nextsquare;
    __uint128_t nearest_sq = (ndiff0 <= ndiff1) ? square : nextsquare;

    /* ---- nearest perfect cube ---- */
    idx_t r = cube_root_floor(n);
    idx_t r_next = r + 1;
    __uint128_t cube = (__uint128_t)r * r * r;
    __uint128_t nextcube = (__uint128_t)r_next * r_next * r_next;
    __uint128_t cdiff0 = (cube <= n) ? n - cube : cube - n;
    __uint128_t cdiff1 = (nextcube >= n) ? nextcube - n : n - nextcube;
    __uint128_t nearest_cu = (cdiff0 <= cdiff1) ? cube : nextcube;

    /* ---- absolute difference ---- */
    __uint128_t diff;
    if (nearest_sq >= nearest_cu)
        diff = nearest_sq - nearest_cu;
    else
        diff = nearest_cu - nearest_sq;

    return (seq_t)diff;
}
