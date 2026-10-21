/*
 * A063520  Sum divides product: number of solutions (r,s,t), r>=s>=t>0, to
 *          the equation rst = n(r+s+t).
 * https://oeis.org/A063520
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of solutions (r,s,t) of * rst = n (r+s+t) with r ≥
 *            s ≥ t > 0
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   _isqrt_u64
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/_isqrt_u64.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    if (n == 0) return count;           /* for safety, though n starts at 1 */

    /* All indices and bounds are in idx_t (unsigned 64‑bit). */
    idx_t limit = 3 * n;                 /* s·t ≤ 3n from r ≥ s ≥ t > 0 */
    idx_t t_max = _isqrt_u64(limit);     /* floor sqrt(limit) */

    for (idx_t t = 1; t <= t_max; ++t) {
        idx_t s_max = limit / t;         /* maximal s for this t */
        if (s_max < t) continue;         /* would violate s ≥ t */

        for (idx_t s = t; s <= s_max; ++s) {
            unsigned __int128 prod = (unsigned __int128)s * t;   /* s·t */
            if (prod <= n) continue;                           /* need st > n */
            if (prod > (unsigned __int128)limit) break;         /* product too large */

            unsigned __int128 denom = prod - n;                 /* st – n > 0 */
            unsigned __int128 numer = (unsigned __int128)n * (s + t); /* n(s+t) */

            if (numer % denom != 0) continue;                  /* r must be integer */
            unsigned __int128 r = numer / denom;               /* candidate r */

            if (r < s) continue;                               /* must have r ≥ s */

            ++count;                                            /* valid triple found */
        }
    }

    return count;
}
