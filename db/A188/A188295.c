/*
 * A188295  a(n) = [n*r]-[n*r-r], where r=1/sqrt(2), [ ]=floor.
 * https://oeis.org/A188295
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = ⌊n/√2⌋ - ⌊(n-1)/√2⌋ (offset 1)
 * Range:     u8 n <= 5, u16 n <= 90, u32 n <= 123, u64 n <= 123,
 *            u128 n <= 123, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"

static seq_t floor_div_sqrt2(idx_t k)
{
    /* Compute ⌊k/√2⌋ using integer arithmetic:
       ⌊k/√2⌋ = ⌊⌊k*√2⌋/2⌋ = floor_sqrt(2*k*k) / 2
    */
    seq_t kk = (seq_t)k;
    seq_t sq = kk * kk;                 /* k² */
    seq_t m  = sq * (seq_t)2;            /* 2*k² */
    seq_t s  = floor_sqrt(m);            /* ⌊√(2*k²)⌋ = ⌊k*√2⌋ */
    return s >> 1;                       /* ⌊⌊k*√2⌋/2⌋ = ⌊k/√2⌋ */
}

seq_t a(idx_t n)
{
    seq_t fn  = floor_div_sqrt2(n);
    seq_t fn1 = floor_div_sqrt2(n - 1);  /* n ≥ 1 → n-1 fits in idx_t */
    return fn - fn1;
}
