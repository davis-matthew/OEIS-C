/*
 * A194173  a(n) = [Sum_{k=1..n} (k*r)], where [ ]=floor, ( )=fractional part,
 *          and r=sqrt(6).
 * https://oeis.org/A194173
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor( Sum_{k=1..n} (k*r) ) for r = sqrt(6)
 * Range:     u8 n <= 2, u16 n <= 9, u32 n <= 364, u64 n <= 5000,
 *            u128 n <= 5000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* t = n*(n+1)/2  (as seq_t) */
    seq_t t = (seq_t)n * ((seq_t)n + (seq_t)1) / (seq_t)2;

    /* A = floor( sqrt( 6 * t^2 ) )  i.e. floor( r * t ) */
    seq_t t_sq = t * t;            /* t^2   */
    seq_t six_t_sq = t_sq * (seq_t)6;  /* 6 * t^2 */
    seq_t A = floor_sqrt(six_t_sq);

    /* sum of floor(k*r) for k = 1..n */
    seq_t sum = (seq_t)0;
    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t k_sq = (seq_t)k * (seq_t)k;  /* k^2  */
        seq_t val  = k_sq * (seq_t)6;      /* 6*k^2 */
        seq_t fk   = floor_sqrt(val);     /* floor(k*sqrt(6)) */
        sum += fk;
    }

    return A - sum;
}
