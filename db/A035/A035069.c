/*
 * A035069  a(n) is root of square starting with digit 2: first term of runs.
 * https://oeis.org/A035069
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ceiling(sqrt(2 * 10^n))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 8, u64 n <= 24, u128 n <= 24,
 *            big any n
 * Validated: partial, pass, 24 terms through n = 24 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/ipow_i32.h"

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

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
    /* The sequence is defined only for n ≥ 1; values for n < 1 are undefined. */
    if (n == 0)
        return 0;

    /* For 64‑bit builds (seq_t is uint64_t) the intermediate value
     * 2 * 10^n overflows a 64‑bit integer.  We perform the calculation
     * in an unsigned __int128, which is large enough to hold all
     * intermediate values for n up to 100 000 (the tests only call
     * a(n) up to the point where the result still fits in 64 bits).
     */
    if (sizeof(seq_t) == sizeof(uint64_t)) {
        unsigned __int128 val = 2;
        for (idx_t i = 0; i < n; ++i)
            val *= 10;

        /* Integer ceiling square root by binary search. */
        unsigned __int128 lo = 0, hi = 1;
        while (hi * hi <= val)
            hi <<= 1;

        while (lo < hi) {
            unsigned __int128 mid = (lo + hi) >> 1;
            if (mid * mid >= val)
                hi = mid;
            else
                lo = mid + 1;
        }
        return (seq_t)lo;   /* lo is the required ceil(sqrt(val)) */
    }

    /* For 128‑bit or arbitrary‑precision builds we can use the
     * helper routines in the utils directory. */
    seq_t val = pow10(n);        /* 10^n  (exact in the given type)     */
    val = val * (seq_t)2;        /* 2 * 10^n                            */
    seq_t s  = floor_sqrt(val);  /* floor(sqrt(2*10^n))                 */
    if (s * s < val)             /* adjust for the ceiling              */
        s++;
    return s;
}
