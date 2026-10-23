/*
 * A188090  [nr+kr]-[nr]-[kr], where r=sqrt(3), k=5, [ ]=floor.
 * https://oeis.org/A188090
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n+5)*sqrt(3)) - floor(n*sqrt(3)) - 8
 *            (equivalently, a(n)=1 iff the fractional part of n*sqrt(3) ≥
 *            1‑frac(5*sqrt(3)), else 0)
 * Range:     u8 none, u16 n <= 68, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Compute floor(n*sqrt(3)) as floor_sqrt(3*n*n)  */
    seq_t nn = (seq_t)n;
    seq_t sq_n = floor_sqrt((seq_t)3 * nn * nn);

    /* Compute floor((n+5)*sqrt(3)) as floor_sqrt(3*(n+5)^2) */
    nn += (seq_t)5;
    seq_t sq_n5 = floor_sqrt((seq_t)3 * nn * nn);

    return sq_n5 - sq_n - (seq_t)8;
}
