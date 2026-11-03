/*
 * A153158  a(n) = A007916(n)^2.
 * https://oeis.org/A153158
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (k)^2 where k is the n-th positive integer that is not a
 *            perfect power
 * Range:     u8 n <= 11, u16 n <= 233, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 65 terms through n = 9487 (2026-11-01)
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

seq_t a(idx_t n)
{
    /* Count non‑perfect‑power integers starting from 2. */
    idx_t count = 0;
    idx_t k = 2;                       /* candidate integer */

    for (;; ++k)
    {
        /* Check whether k is a perfect power. */
        bool is_pp = false;
        seq_t k_seq = (seq_t)k;
        idx_t limit = (idx_t)floor_sqrt(k_seq);   /* integer sqrt of k */

        for (idx_t base = 2; base <= limit && !is_pp; ++base)
        {
            seq_t val = (seq_t)base * base;       /* base^2 */
            /* Multiply until val >= k or next multiplication would exceed k */
            while (val < k_seq && val <= k_seq / (seq_t)base)
            {
                val *= base;
            }
            if (val == k_seq)
            {
                is_pp = true;
                break;
            }
        }

        if (!is_pp)                /* k is not a perfect power */
        {
            ++count;
            if (count == n)
                return (seq_t)k * k;   /* return square of the k-th non‑perfect‑power */
        }
    }
    /* Unreachable – required for compilers that warn about missing return. */
    return 0;
}
