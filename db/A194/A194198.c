/*
 * A194198  [sum{(k*r) : 1<=k<=n}], where [ ]=floor, ( )=fractional part, and
 *          r=sqrt(10).
 * https://oeis.org/A194198
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = floor((n*(n+1)/2)*sqrt(10)) - Σ_{k=1}^n floor(k*sqrt(10))
 * Range:     u8 n <= 1, u16 n <= 8, u32 n <= 1211, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* include the standard integer square‑root utility */
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    if (n == 0) {
        /* Offset starts at 1, but returning 0 for n==0 is harmless. */
        return (seq_t)0;
    }

    /* x = n*(n+1)/2 as seq_t */
    seq_t x = (seq_t)n * ((seq_t)n + (seq_t)1) / (seq_t)2;

    /* floor_T = floor(x * sqrt(10)) = floor_sqrt(10 * x * x) */
    seq_t floor_T = floor_sqrt((seq_t)10 * x * x);

    /* sum_floor = Σ_{k=1}^n floor(k*sqrt(10)) */
    seq_t sum_floor = (seq_t)0;
    for (idx_t k = (idx_t)1; k <= n; ++k) {
        seq_t k_sq = (seq_t)k * (seq_t)k;
        seq_t floor_k = floor_sqrt((seq_t)10 * k_sq);
        sum_floor += floor_k;
    }

    /* a(n) = floor_T - sum_floor */
    return floor_T - sum_floor;
}
