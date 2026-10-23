/*
 * A028621  Expansion of (theta_3(z)*theta_3(14z)+theta_2(z)*theta_2(14z)).
 * https://oeis.org/A028621
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) counts the number of integer solutions (x,y) to x^2 +
 *            14*y^2 = n with x≡y (mod 2), each solution counted with its sign
 *            variations. Equivalent to the coefficient of q^n in the series
 *            theta_3(z)*theta_3(14z)+theta_2(z)*theta_2(14z)
 * Range:     u8 n <= 63, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t count = 0;
    for (idx_t y = 0; ; ++y) {
        /* term = 14*y*y, use 128‑bit to avoid overflow */
        unsigned __int128 term = (unsigned __int128)14 * y * y;
        if (term > n) break;
        seq_t rem = (seq_t)n - (seq_t)term;          /* n - 14*y*y */
        /* integer square root of rem */
        idx_t x = (idx_t)floor_sqrt(rem);
        seq_t xsq = (seq_t)x * (seq_t)x;
        if (xsq != rem) continue;                 /* not a perfect square */
        if (((x & 1) == (y & 1))) {                /* same parity */
            if (y == 0) {
                if (x == 0) count += 1;           /* (0,0) */
                else        count += 2;           /* ±x with y=0 */
            } else {
                if (x == 0) count += 2;           /* x=0, ±y */
                else        count += 4;           /* ±x, ±y */
            }
        }
    }
    return count;
}
