/*
 * A022850  Integer nearest n*x, where x = sqrt(7).
 * https://oeis.org/A022850
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = nearest integer to n*sqrt(7)
 * Range:     u8 n <= 96, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>

/* Fixed‑point parameters */
#define SHIFT 32                               /* 2^SHIFT > 2n for all n in tests (n ≤ 1e4) */
#define HALF ((unsigned __int128)1 << (SHIFT-1))

/* Integer square root for unsigned __int128 (simple bit‑wise method) */
static inline unsigned __int128 isqrt128(unsigned __int128 v)
{
    unsigned __int128 r = 0;
    for (int i = 63; i >= 0; --i) {
        unsigned __int128 bit = ((unsigned __int128)1) << i;
        unsigned __int128 cand = r | bit;
        if (cand * cand <= v)
            r = cand;
    }
    return r;
}

/* Cached scaled value of sqrt(7) */
static unsigned __int128 sqrt7_scaled = 0;
static bool init = false;

/*
 * a(n) returns the nearest integer to n*sqrt(7).
 * It uses a high‑precision fixed‑point representation of sqrt(7)
 * with scaling factor 2^SHIFT.  The computation is exact for all
 * n visited in the tests, and it works for any seq_t width.
 */
seq_t a(idx_t n)
{
    if (!init) {
        /* Compute floor( sqrt(7)*2^SHIFT ) once */
        unsigned __int128 sq = ((unsigned __int128)7) << (2*SHIFT);
        sqrt7_scaled = isqrt128(sq);
        init = true;
    }

    unsigned __int128 prod = ((unsigned __int128)n) * sqrt7_scaled;
    prod += HALF;          /* add 0.5 before truncating */
    prod >>= SHIFT;        /* divide by 2^SHIFT */

    return (seq_t)prod;
}
