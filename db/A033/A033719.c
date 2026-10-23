/*
 * A033719  Coefficients in expansion of theta_3(q) * theta_3(q^7) in powers
 *          of q.
 * https://oeis.org/A033719
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer pairs (x, y) with x^2 + 7*y^2 = n
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   fast_isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/fast_isqrt_u64.h"

/* Fast integer square‑root for 0 ≤ n < 2^64 */

/* Return a(n) for the OEIS sequence A033719. */
seq_t a(idx_t n)
{
    seq_t sum = 0;
    idx_t ymax = fast_isqrt_u64(n / 7u);   /* maximum |y| for which 7*y^2 ≤ n */

    for (idx_t y = 0; y <= ymax; ++y) {
        unsigned __int128 prod128 = (unsigned __int128)7 * y * y;
        if (prod128 > n) break;                   /* safety guard */
        idx_t t = n - (idx_t)prod128;
        idx_t s = fast_isqrt_u64(t);
        if ((unsigned __int128)s * s == t) {       /* t is a perfect square */
            seq_t cntx = (s == 0) ? 1 : 2;
            seq_t cnty = (y == 0) ? 1 : 2;
            sum += cntx * cnty;                    /* 1, 2 or 4 solutions */
        }
    }
    return sum;
}
