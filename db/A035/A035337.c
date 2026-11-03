/*
 * A035337  Third column of Wythoff array.
 * https://oeis.org/A035337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3*floor((n+1)*φ) + 2*(n+1) - 2 where φ = (1+√5)/2. This
 *            equals 3*A000201(n+1)+2*(n+1)-2
 * Range:     u8 n <= 2, u16 n <= 45, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 62 terms through n = 9847 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Floor square root by binary search */
static inline seq_t floor_sqrt(seq_t x)
{
    seq_t lo = 0, hi = 1;
    while (ipow_i32(hi, 2) <= x)
        hi <<= 1;
    while (lo < hi)
    {
        seq_t mid = (lo + hi) >> 1;
        seq_t mid2 = mid * mid;
        if (mid2 <= x)
            lo = mid + 1;
        else
            hi = mid;
    }
    return lo - 1;
}

/* Compute floor(k*φ) for k >= 1 using integer arithmetic:
   floor(k*φ) = floor((k + floor(k*√5)) / 2) =
                (k + floor_sqrt(5*k*k)) / 2.  */
static seq_t beatty_phi(idx_t k)
{
    if (k == 0) return 0;          /* safety, though k>=1 in usage */
    seq_t kk = (seq_t)k;
    seq_t t  = (seq_t)5 * kk * kk;   /* 5*k^2   */
    seq_t s  = floor_sqrt(t);        /* floor(k*√5) */
    return (kk + s) / 2;
}

seq_t a(idx_t n)
{
    idx_t k = n + 1;                  /* index shift to match A000201 */
    seq_t kk = (seq_t)k;
    seq_t val = beatty_phi(k);        /* floor((n+1)*φ) */
    seq_t res = (seq_t)3 * val + (seq_t)2 * kk - (seq_t)2;
    return res;
}
