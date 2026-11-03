/*
 * A166065  Triangle, read by rows, given by [0,1,1,0,0,0,0,0,0,0,...] DELTA
 *          [2,-1,0,0,0,0,0,0,0,...] where DELTA is the operator defined in
 *          A084938.
 * https://oeis.org/A166065
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = 1 for n = 0; otherwise * let m = floor((sqrt(8n+1)-1)/2)
 *            (the largest m with T(m)=m(m+1)/2 ≤ n), * let i = n - T(m). * if
 *            i == 0 (i.e., n is a triangular number >0) then a(n)=0, *
 *            otherwise let exp = m - i; if exp < 1 set exp = 1. * Then a(n) =
 *            2^exp
 * Range:     u8 n <= 7, u16 n <= 80, u32 n <= 80, u64 n <= 80, u128 n <= 80,
 *            big any n
 * Validated: partial, pass, 81 terms through n = 80 (2026-11-01)
 * Helpers:   ipow_i32, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>
#include "../utils/ipow_i32.h"
#include "../utils/pow2.h"

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

seq_t a(idx_t n)
{
    if (n == 0)            /* a(0) = 1 */
        return (seq_t)1;

    /* find   m = floor((sqrt(8n+1)-1)/2)  */
    seq_t t = (seq_t)8 * (seq_t)n + 1;      /* 8*n+1 (may need >64‑bit)   */
    seq_t sq = floor_sqrt(t);               /* floor(sqrt(8n+1))          */
    idx_t m = (idx_t)((sq - 1) / 2);        /* largest m with T(m) ≤ n    */

    /* triangular number T(m) = m(m+1)/2  */
    seq_t t_m = (seq_t)m * ((seq_t)m + 1) / 2;
    idx_t i = n - (idx_t)t_m;               /* distance to the next triangle */

    if (i == 0)                /* n is a triangular number (≥1) */
        return (seq_t)0;

    idx_t exp = m - i;         /* exponent for 2 */
    if (exp < 1)
        exp = 1;
    return pow2(exp);          /* 2^exp */
}
