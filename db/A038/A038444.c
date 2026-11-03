/*
 * A038444  Sums of 2 distinct powers of 10.
 * https://oeis.org/A038444
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 10, u32 n <= 45, u64 n <= 190, u128 n <= 741,
 *            big any n
 * Validated: standard, pass, 47 terms through n = 9618 (2026-11-01)
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* OEIS A038444: Sums of two distinct powers of 10
   Formula: a(n) = 10^i + 10^j
   where j is the smallest integer with j*(j+1)/2 >= n
   and i = n - j*(j-1)/2 - 1 */
#include <stdint.h>

/* Compute 10^e for a non‑negative integer exponent e */
static seq_t pow10(idx_t e) {
    seq_t res = 1;
    while (e) {
        res *= 10;
        e--;
    }
    return res;
}

seq_t a(idx_t n)
{
    /* Find the smallest j such that j*(j+1)/2 >= n */
    idx_t lo = 1, hi = 1;
    while ((unsigned __int128)hi * (hi + 1) / 2 < n)
        hi <<= 1;             /* double hi until the bound holds */

    /* Binary search in [lo, hi] */
    while (lo < hi) {
        idx_t mid = lo + (hi - lo) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            hi = mid;
        else
            lo = mid + 1;
    }

    idx_t j = lo;                                 /* exponent of the larger power */
    idx_t prev_count = (j - 1) * j / 2;            /* number of terms before this j-block */
    idx_t offset = n - prev_count;                /* 1‑based index inside the block */
    idx_t i = offset - 1;                          /* exponent of the smaller power */

    return pow10(j) + pow10(i);
}
