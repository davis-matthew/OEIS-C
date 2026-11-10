/*
 * A079828  Duplicate of A061229.
 * https://oeis.org/A079828
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  dead
 * Formula:   a(n) = floor(sqrt(n * rev(n))), rev(n) = decimal digit reversal
 *            of n
 * Range:     i8 n <= 7, i16 n <= 27, i32 n <= 27, i64 n <= 27, i128 n <= 27,
 *            big any n
 * Validated: partial, pass, 27 terms through n = 27 (2026-11-07)
 * Helpers:   ipow_i32, reverse_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/ipow_i32.h"
#include "../utils/reverse_digits_idx.h"

static inline seq_t A061229_floor_sqrt(seq_t x)
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

static seq_t A061229_a(idx_t n)
{
    if (n == 0) return 0;
    idx_t reverse_n = reverse_digits_idx(n);
    seq_t product = (seq_t)n * (seq_t)reverse_n;
    return A061229_floor_sqrt(product);
}

seq_t a(idx_t n)
{
    return A061229_a(n);
}
