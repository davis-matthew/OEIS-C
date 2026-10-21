/*
 * A092573  Number of solutions to x^2 + 3y^2 = n in positive integers x and
 *          y.
 * https://oeis.org/A092573
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #{(x,y) ∈ ℕ⁺² : x² + 3y² = n}
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

/* Integer square root for an unsigned 64‑bit integer (floor(√n)).  Very
 * fast and works for all n that fit in idx_t. */
static inline idx_t isqrt_u64(idx_t n)
{
    if (n <= 1) return n;
    idx_t x = n;
    idx_t y = (x + 1) >> 1;
    while (y < x) {
        x = y;
        y = (x + n / x) >> 1;
    }
    return x;
}

seq_t a(idx_t n)
{
    seq_t count = 0;

    /* We iterate over y=1,2,… until 3*y*y >= n.  Using an
     * unsigned __int128 intermediate avoids overflow on 64‑bit
     * builds when n is large. */
    for (idx_t y = 1; ; ++y) {
        unsigned __int128 prod = (unsigned __int128) y * y * 3;
        if (prod >= n) {
            /* Exactly equal ⇒ rem = 0 ⇒ x = 0, not allowed. */
            if (prod == n) continue;
            break;       /* Further y will only increase prod. */
        }

        /* Remaining part of the equation: x² = n – 3y² > 0. */
        idx_t rem = n - (idx_t)prod;   /* rem > 0 */
        idx_t x = isqrt_u64(rem);
        if (x * x == rem) ++count;
    }

    return count;
}
