/*
 * A188068  a(n) = [n*r]-[k*r]-[n*r-k*r], where r=sqrt(3), k=1, [ ]=floor.
 * https://oeis.org/A188068
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor(n*sqrt(3)) – floor(sqrt(3)) – floor((n–1)*sqrt(3))
 *            Since floor(sqrt(3)) = 1, this simplifies to a(n) =
 *            floor(n*sqrt(3)) – 1 – floor((n–1)*sqrt(3)). The function
 *            computes a(n) by integer arithmetic using the built‑in
 *            floor_sqrt routine
 * Range:     u8 n <= 4, u16 n <= 73, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* floor(n*sqrt(3)) = floor(sqrt(3*n^2)) */
    seq_t nn   = (seq_t)n;
    seq_t m1   = nn * nn * (seq_t)3;
    seq_t f1   = floor_sqrt(m1);

    /* floor((n-1)*sqrt(3)) = floor(sqrt(3*(n-1)^2)) */
    idx_t n1i  = n - (idx_t)1;
    seq_t n1   = (seq_t)n1i;
    seq_t m2   = n1 * n1 * (seq_t)3;
    seq_t f2   = floor_sqrt(m2);

    /* a(n) = f1 - 1 - f2 */
    return f1 - (seq_t)1 - f2;
}
