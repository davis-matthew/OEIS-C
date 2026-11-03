/*
 * A236384  Number of non-congruent integer triangles with base length n whose
 *          apex lies on or within a space bounded by a semicircle of diameter
 *          n.
 * https://oeis.org/A236384
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = number of integer-sided obtuse or right triangles * with
 *            largest side n
 * Range:     u8 n <= 9, u16 n <= 116, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Validated: standard, pass, 75 terms through n = 977 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  a(n) = A002620(n+1) - A247588(n)  */

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
    if (n < 3u)
        return 0;

    seq_t count = 0;
    seq_t n_seq = (seq_t)n;
    seq_t n_sq  = n_seq * n_seq;

    /* smallest possible a is ceil(n/2) = (n+1)/2 */
    for (idx_t a = (n + 1) / 2; a < n; ++a)
    {
        seq_t a_seq = (seq_t)a;
        seq_t diff   = n_sq - a_seq * a_seq;

        seq_t b_max = floor_sqrt(diff);      /* floor(sqrt(n^2 - a^2)) */

        if (b_max > a_seq)
            b_max = a_seq;                   /* enforce a <= b */

        seq_t b_min = n_seq - a_seq + (seq_t)1;  /* triangle inequality */

        if (b_max >= b_min)
            count += b_max - b_min + (seq_t)1;
    }

    return count;
}
