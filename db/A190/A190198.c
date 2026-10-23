/*
 * A190198  a(n) = [n*u+n*v]-[n*u]-[n*v], where u=sqrt(3), v=1/u, and
 *          []=floor.
 * https://oeis.org/A190198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = [n*u+n*v]-[n*u]-[n*v]
 * Range:     u8 n <= 1, u16 n <= 18, u32 n <= 4516, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

/* seq_t a(idx_t n)
   Computes a(n) = floor(n*sqrt(3)+n/sqrt(3)) - floor(n*sqrt(3)) - floor(n/sqrt(3))
   using only integer arithmetic. */
seq_t a(idx_t n) {
    if (n == 0) return 0;                     /* not part of the OEIS offset, but safe */
    seq_t nn = (seq_t)n;
    /* floor(n*sqrt(3)) = floor(sqrt(3*n^2)) */
    seq_t s = floor_sqrt((seq_t)3 * nn * nn);
    /* floor(n*v) = floor(n/sqrt(3)) = floor(s/3) */
    seq_t r = s / 3;
    /* floor((4n*sqrt(3))/3) = floor(sqrt(48*n^2)/3) */
    seq_t t = floor_sqrt((seq_t)48 * nn * nn) / 3;
    return t - s - r;
}
