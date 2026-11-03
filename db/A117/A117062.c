/*
 * A117062  Hexagonal numbers for which the sum of the digits is also a
 *          hexagonal number.
 * https://oeis.org/A117062
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  base, nonn
 * Formula:   not stated
 * Range:     u8 n <= 3, u16 n <= 35, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 55 terms through n = 998 (2026-11-01)
 * Helpers:   digit_sum, ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Implements A117062: hexagonal numbers whose digit sum is also hexagonal.
 *  a(0) = 0, a(1) = 1, a(2) = 6, a(3) = 15, …  (see OEIS entry).
 *  For n > 0, the value is the n‑th hexagonal number h = k·(2k−1) such that
 *  digit_sum(h) is itself a hexagonal number.  The result is computed on
 *  demand; no static array of seq_t objects is allocated.
 */

#include <stdbool.h>
#include <stdlib.h>
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

/*  Helper: test if an integer is a hexagonal number. */
static bool is_hexagonal(seq_t x)
{
    if (x == 0) return true;
    seq_t w = (seq_t)8u * x + (seq_t)1u;
    seq_t t = floor_sqrt(w);
    return (t * t == w) && ((t + (seq_t)1u) % (seq_t)4u == 0);
}

/*  The sequence function. */
seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;          /* a(0) = 0 */
    idx_t count = 1;                      /* already counted the 0 term */
    for (idx_t k = 1;; ++k) {
        seq_t kseq = (seq_t)k;
        seq_t h = kseq * ((seq_t)2u * kseq - (seq_t)1u);
        if (is_hexagonal(digit_sum(h))) {
            if (count == n) return h;
            ++count;
        }
        /* The loop will terminate before overflow for n up to 10000
           (and in practice much earlier). */
    }
}
