/*
 * A190509  a(n) = n + [nr/s] + [nt/s] + [nu/s] where r=golden ratio, s=r^2,
 *          t=r^3, u=r^4, and [] represents the floor function.
 * https://oeis.org/A190509
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [n/r] + [n*t/s] + [n*u/s] (r = (1+√5)/2, s = r^2, t =
 *            r^3, u = r^4)
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Equivalent integer‑only expression:
   a(n) = floor(n/φ) + 2*n + 2*floor(n*φ)
   where φ = (1+√5)/2.
   floor(n*φ) = (n + floor_sqrt(5*n^2)) / 2
   floor(n/φ) = (floor_sqrt(5*n^2) - n) / 2
*/

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    seq_t nn = (seq_t)n;                      /* cast index to seq_t */
    seq_t sqrt5_n2 = floor_sqrt((seq_t)5 * nn * nn); /* floor(n*√5) */

    /* floor(n*φ) = (n + floor(sqrt(5)*n)) / 2  */
    seq_t w = (nn + sqrt5_n2) / 2;

    /* floor(n/φ) = (floor(sqrt(5)*n) – n) / 2  */
    seq_t v = (sqrt5_n2 - nn) / 2;

    /* a(n) = floor(n/φ) + 2*n + 2*floor(n*φ) */
    return v + 2*nn + 2*w;
}
