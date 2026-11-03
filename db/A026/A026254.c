/*
 * A026254  a(n) = j if n = [ j*sqrt(3) ], else a(n) = k if n = [ (k/2)*(3 +
 *          sqrt(3)) ].
 * https://oeis.org/A026254
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = j if n = ⌊j·√3⌋, else a(n) = k if n = ⌊k·(3+√3)/2⌋,
 *            offset: 1
 * Range:     u8 n <= 5, u16 n <= 66, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 82 terms through n = 9475 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
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

static seq_t floor_k_sqrt3(idx_t k)
{
    seq_t kk = (seq_t)k * (seq_t)k;   /* k² */
    kk = kk * (seq_t)3;              /* 3·k² */
    return floor_sqrt(kk);           /* ⌊√(3·k²)⌋ = ⌊k·√3⌋ */
}

static seq_t floor_k_beta(idx_t k)
{
    seq_t s = floor_k_sqrt3(k);                      /* ⌊k·√3⌋ */
    seq_t term = (seq_t)3 * (seq_t)k + s;           /* 3k + ⌊k·√3⌋ */
    return term / (seq_t)2;                         /* ⌊(3k + ⌊k·√3⌋)/2⌋ = ⌊k·(3+√3)/2⌋ */
}

seq_t a(idx_t n)
{
    if (n < 1)
        return (seq_t)0;          /* undefined for n < 1, fallback */

    seq_t target = (seq_t)n;

    /* Search the √3 Beatty sequence */
    idx_t lo = 1, hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        seq_t val = floor_k_sqrt3(mid);
        if (val == target)
            return (seq_t)mid;
        else if (val < target)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    /* Search the (3+√3)/2 Beatty sequence */
    lo = 1; hi = n;
    while (lo <= hi) {
        idx_t mid = lo + ((hi - lo) >> 1);
        seq_t val = floor_k_beta(mid);
        if (val == target)
            return (seq_t)mid;
        else if (val < target)
            lo = mid + 1;
        else
            hi = mid - 1;
    }

    return (seq_t)0;      /* should never reach here */
}
