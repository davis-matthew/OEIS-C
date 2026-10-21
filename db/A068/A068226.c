/*
 * A068226  Neighbor in 2-1 direction of numbers arranged as clockwise spiral.
 * https://oeis.org/A068226
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the integer immediately to the left of n in the standard
 *            clockwise Ulam spiral (1 at the centre, 2 to the right of 1)
 * Range:     u8 n <= 196, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* integer square root: floor(sqrt(x)) */
static inline idx_t isqrt(idx_t x)
{
    if (!x) return 0;
    idx_t r = 0;
    for (int i = 63 - __builtin_clzll(x); i >= 0; --i) {
        idx_t tmp = r | ((idx_t)1 << i);
        unsigned __int128 sq = (unsigned __int128)tmp * tmp;
        if (sq <= x) r = tmp;
    }
    return r;
}

/* ceil odd square-root: smallest odd t >= sqrt(x) */
static inline idx_t ceil_odd_sqrt(idx_t x)
{
    idx_t r = isqrt(x);
    if ((unsigned __int128)r * r < x) ++r;
    if ((r & 1) == 0) ++r;
    return r;
}

/* Convert index n to (x,y) coordinates.  Coordinates are 64‑bit signed. */
static void n_to_xy(idx_t n, int64_t *x, int64_t *y)
{
    if (n == 1) {
        *x = 0;
        *y = 0;
        return;
    }

    idx_t ring = (ceil_odd_sqrt(n) - 1) / 2;
    idx_t side = ring * 2;
    unsigned __int128 max_val = (unsigned __int128)(2 * ring + 1) * (2 * ring + 1);
    unsigned __int128 offset128 = max_val - n;
    idx_t offset = (idx_t)offset128;   /* offset < 4*side */

    if (offset < side) {                     /* bottom side */
        *x = (int64_t)(ring - offset);
        *y = -(int64_t)ring;
    } else if (offset < 2 * side) {          /* left side */
        *x = -(int64_t)ring;
        *y = -(int64_t)ring + (int64_t)(offset - side);
    } else if (offset < 3 * side) {          /* top side */
        *x = -(int64_t)ring + (int64_t)(offset - 2 * side);
        *y = (int64_t)ring;
    } else {                                 /* right side */
        *x = (int64_t)ring;
        *y = (int64_t)ring - (int64_t)(offset - 3 * side);
    }
}

/* Convert coordinates (x,y) to index n. */
static inline seq_t xy_to_n(int64_t x, int64_t y)
{
    int64_t ax = x > 0 ? x : -x;
    int64_t ay = y > 0 ? y : -y;
    int64_t ring = ax > ay ? ax : ay;
    if (ring == 0) return (seq_t)1;

    int64_t side = ring * 2;
    unsigned __int128 max_val = (unsigned __int128)(2 * ring + 1) * (2 * ring + 1);

    idx_t offset;
    if (y == -ring) {                         /* bottom side */
        offset = ring - x;
    } else if (x == -ring) {                  /* left side */
        offset = side + (y + ring);
    } else if (y == ring) {                   /* top side */
        offset = 2 * side + (x + ring);
    } else {                                  /* right side */
        offset = 3 * side + (ring - y);
    }

    unsigned __int128 n128 = max_val - offset;
    return (seq_t)n128;
}

/* Public entry point */
seq_t a(idx_t n)
{
    int64_t x, y;
    n_to_xy(n, &x, &y);
    return xy_to_n(x - 1, y);
}
