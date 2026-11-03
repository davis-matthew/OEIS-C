/*
 * A061229  a(n) = floor of the geometric mean of n and the reversal of n.
 * https://oeis.org/A061229
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = floor(sqrt(n * rev(n)))
 * Range:     u8 n <= 7, u16 n <= 425, u32 n <= 2000, u64 n <= 2000,
 *            u128 n <= 2000, big any n
 * Validated: standard, pass, 88 terms through n = 1997 (2026-11-01)
 * Helpers:   ipow_i32, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/reverse_digits_idx.h"

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
    if (n == 0) return 0;
    idx_t reverse_n = reverse_digits_idx(n);
    seq_t product = (seq_t)n * (seq_t)reverse_n;
    return floor_sqrt(product);
}
