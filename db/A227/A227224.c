/*
 * A227224  Numbers k such that k*(sum of digits of k) is a perfect square.
 * https://oeis.org/A227224
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 43, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Validated: partial, pass, 62 terms through n = 62 (2026-11-01)
 * Helpers:   digit_sum, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A227224: numbers k such that k * digit_sum(k) is a perfect square */

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

seq_t a(idx_t n)
{
    idx_t count = 0;
    for (idx_t k = 0;; ++k) {
        seq_t ks   = (seq_t)k;                /* current number as seq_t      */
        seq_t ssum = digit_sum(ks);           /* sum of digits of ks          */
        seq_t prod = ks * ssum;               /* k * sumdigits                */
        seq_t root = floor_sqrt(prod);        /* integer floor of sqrt(prod)  */
        if (root * root == prod) {            /* perfect square check         */
            ++count;
            if (count == n)
                return ks;                    /* found the nth term            */
        }
    }
}
