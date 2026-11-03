/*
 * A118488  Squares for which the sum of the digits is a triangular number.
 * https://oeis.org/A118488
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 25, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 53 terms through n = 8596 (2026-11-01)
 * Helpers:   digit_sum, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A118488: Squares for which the sum of the digits is a triangular number.
   a(n) is the n-th smallest such square. */
#include <stdbool.h>
#include <stdint.h>
#include "../utils/digit_sum.h"
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

seq_t a(idx_t n) {
    /* If the index is zero we simply return 0 – the offset is 1. */
    if (n == 0) return (seq_t)0;

    seq_t cnt = 0;        /* how many qualifying squares we have found */
    seq_t k   = 0;        /* current integer whose square we test */
    seq_t sq  = 0;        /* current square */

    while (cnt < n) {
        sq = k * k;                  /* compute k^2 in seq_t */
        seq_t ds = digit_sum(sq);    /* sum of decimal digits of sq */

        /* Test whether ds is triangular:
           ds = t*(t+1)/2  <=>  8*ds + 1 = (2t+1)^2 */
        seq_t D = 8 * ds + 1;
        seq_t t = floor_sqrt(D);
        bool tri = (t * t == D) && ((t - 1) & 1) == 0;

        if (tri) {
            cnt++;
            /* keep the last qualifying square – it will be returned when cnt == n */
        }
        k++;
    }
    return sq;
}
