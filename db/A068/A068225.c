/*
 * A068225  Neighbor in 1-2 direction of numbers arranged as clockwise spiral.
 * https://oeis.org/A068225
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 225, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 88 terms through n = 9886 (2026-11-01)
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the sequence by determining the coordinates of n in the
   Ulam spiral and returning the number immediately to the right
   (the 1‑2 direction). */

#include <stdint.h>
#include "../utils/isqrt.h"

/* Convert n to (x, y) coordinates in the Ulam spiral */
static void n_to_xy(idx_t n, int64_t *x, int64_t *y)
{
    if (n == 1) {
        *x = 0;
        *y = 0;
        return;
    }

    /* Find ring level l */
    idx_t sqrt_floor = isqrt(n);
    /* ceil sqrt */
    if ((__int128)sqrt_floor * sqrt_floor < n)
        sqrt_floor++;

    idx_t l = sqrt_floor / 2;
    int64_t L = (int64_t)l;
    int64_t side = 2 * L;
    __int128 max_val = (__int128)(2 * L + 1) * (2 * L + 1);
    __int128 d = max_val - n; /* d is non‑negative and fits 128 bits */

    if (d < side) {
        *x = L - (int64_t)d;
        *y = L;
    } else if (d < 2 * (__int128)side) {
        *x = -L;
        *y = L - (int64_t)(d - side);
    } else if (d < 3 * (__int128)side) {
        *x = -L + (int64_t)(d - 2 * (__int128)side);
        *y = -L;
    } else {
        *x = L;
        *y = -L + (int64_t)(d - 3 * (__int128)side);
    }
}

/* Convert (x, y) coordinates back to n */
static idx_t xy_to_n(int64_t x, int64_t y)
{
    if (x == 0 && y == 0)
        return 1;

    int64_t ax = x >= 0 ? x : -x;
    int64_t ay = y >= 0 ? y : -y;
    int64_t L = ax > ay ? ax : ay;
    int64_t side = 2 * L;
    __int128 max_val = (__int128)(2 * L + 1) * (2 * L + 1);
    __int128 d;

    if (y == L) {                           /* top side */
        d = (int64_t)(L - x);
    } else if (x == -L) {                   /* left side */
        d = (__int128)side + (int64_t)(L - y);
    } else if (y == -L) {                   /* bottom side */
        d = (__int128)(2 * side) + (int64_t)(x + L);
    } else {                                /* right side (x == L) */
        d = (__int128)(3 * side) + (int64_t)(y + L);
    }

    __int128 n128 = max_val - d;
    return (idx_t)n128;
}

/* Main sequence function */
seq_t a(idx_t n)
{
    int64_t x, y;
    n_to_xy(n, &x, &y);

    /* neighbor to the right: (x+1, y) */
    int64_t nx = x + 1;
    int64_t ny = y;

    return (seq_t)xy_to_n(nx, ny);
}
