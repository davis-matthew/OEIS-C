/*
 * A034995  Smallest square starting with a string of n 9's.
 * https://oeis.org/A034995
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = smallest perfect square whose decimal representation
 *            begins with n consecutive 9's
 * Range:     u8 n <= 1, u16 n <= 1, u32 n <= 4, u64 n <= 9, u128 n <= 18,
 *            big any n
 * Validated: standard, pass, 28 terms through n = 483 (2026-11-01)
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
    if (n == 0) return 0;  /* offset starts at 1 */

    /* 10^n and the prefix m = 10^n – 1, i.e. n 9's. */
    seq_t ten_pow_n = pow10(n);
    seq_t prefix    = ten_pow_n - 1;

    /* Search over the number of trailing digits m.
       We need the smallest square k² such that
         prefix·10^m ≤ k² < 10^n·10^m. */
    for (idx_t m = 0; ; ++m) {
        seq_t ten_pow_m = pow10(m);
        seq_t low  = prefix * ten_pow_m;   /* inclusive lower bound */
        seq_t high = ten_pow_n * ten_pow_m;/* exclusive upper bound */

        /* ceil(sqrt(low)) */
        seq_t k = floor_sqrt(low);
        if (k * k < low) ++k;

        seq_t sq = k * k;
        if (sq < high) return sq;          /* first square in the interval */
    }
}
