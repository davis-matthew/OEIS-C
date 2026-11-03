/*
 * A127899  Transform related to the harmonic series.
 * https://oeis.org/A127899
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  tabl, sign, easy
 * Formula:   a(n) = floor((i+2)/(t+2)) * (t+1) * (-1)^(i+t+1), where t =
 *            floor((sqrt(8*n-7)-1)/2), and i = n - t*(t+1)/2. Implements
 *            sequence A127899
 * Range:     i8 n <= 31, i16 n <= 5050, i32 n <= 5050, i64 n <= 5050,
 *            i128 n <= 5050, big any n
 * Validated: standard, pass, 94 terms through n = 4296 (2026-11-01)
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

seq_t a(idx_t n)
{
    if (n == 0) return 0;           /* out‑of‑range guard, not used by OEIS */

    /* --- find t with binary search (t = floor((sqrt(8*n-7)-1)/2)) --- */
    idx_t low  = 0;
    idx_t high;

    /* upper bound: t <= sqrt(2*n) => use integer sqrt of 2*n */
    seq_t tmp = (seq_t)2 * (seq_t)n;
    seq_t sroot = floor_sqrt(tmp);          /* floor(sqrt(2*n)) */
    high = (idx_t)sroot + 1;                /* ensure high > t */

    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;   /* upper mid for correct convergence */
        __int128 prod = (__int128)mid * (mid + 1);
        prod /= 2;
        if (prod < (__int128)n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t t = low;

    /* --- compute i = n - t*(t+1)/2 --- */
    __int128 prod = (__int128)t * (t + 1);
    prod /= 2;
    idx_t i = n - (idx_t)prod;

    /* --- compute ratio = floor((i+2)/(t+2)) --- */
    idx_t ratio = (i + 2) / (t + 2);

    /* --- determine sign: (-1)^(i+t+1) --- */
    seq_t sign = ((i + t + 1) & 1) ? (seq_t)(-1) : (seq_t)1;

    /* --- final result --- */
    seq_t result = (seq_t)ratio * ((seq_t)(t + 1)) * sign;

    return result;
}
