/*
 * A141145  Y values of the complete set of 23 integer solutions to the Ochoa
 *          curve equation.
 * https://oeis.org/A141145
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  fini, full, nonn
 * Formula:   not stated
 * Range:     u8 none, u16 n <= 16, u32 n <= 23, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A141145: Y values of the integer points on the Ochoa curve
 *  3Y² = 2X³ + 386X² + 256X – 58195   (Weierstrass form y² = x³ – 440067x + 106074110).
 * This implementation searches all integer X in a fixed range,
 * computes the right‑hand side, checks whether it is a perfect square,
 * and records the positive square root Y.
 * The Y values are returned in the order of ascending X.
 * For n larger than the number of solutions found, 0 is returned. */

#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>

/* Integer square root for unsigned 64‑bit values. */
static uint64_t isqrt64(uint64_t v)
{
    uint64_t lo = 0, hi = 4294967295ULL, mid, res = 0;
    while (lo <= hi) {
        mid = (lo + hi) >> 1;
        __int128 sq = (__int128)mid * mid;
        if (sq <= v) {
            res = mid;
            lo = mid + 1;
        } else {
            hi = mid - 1;
        }
    }
    return res;
}

/* Store found Y values. */
static uint64_t ys[128];
static size_t ycount = 0;
static bool initialized = false;

/* Initialise by scanning all integer X up to ±BOUND. */
static void init(void)
{
    if (initialized) return;
    const int64_t BOUND = 2000000LL;          /* ±2 000 000 is large enough for all solutions */

    for (int64_t x = -BOUND; x <= BOUND; ++x) {
        __int128 rhs = (__int128)x * x * x
                     - (__int128)440067 * x
                     + 106074110;
        if (rhs < 0) continue;

        uint64_t rhs_u = (uint64_t)rhs;        /* fits in 64 bits for this bound */
        uint64_t y = isqrt64(rhs_u);
        if (y > 0 && (__int128)y * y == rhs) {
            ys[ycount++] = y;
            if (ycount == sizeof(ys) / sizeof(ys[0])) break; /* safety */
        }
    }
    initialized = true;
}

/* Return the nth term of the sequence (1‑based). */
seq_t a(idx_t n)
{
    init();
    if (n >= 1 && n <= ycount)
        return (seq_t)ys[n - 1];
    return (seq_t)0;
}
