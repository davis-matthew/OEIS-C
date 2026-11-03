/*
 * A120867  a(n) is the number k for which there exists a unique pair (j,k) of
 *          positive integers such that (j + k + 1)^2 - 4*k = 20*n^2.
 * https://oeis.org/A120867
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = -5*n^2 + floor(1 + n*sqrt(5))^2
 * Range:     u8 n <= 3, u16 n <= 57, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Validated: standard, pass, 81 terms through n = 9371 (2026-11-01)
 * Helpers:   ipow_i32
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This uses the identity floor(1 + n*sqrt(5)) = floor_sqrt(5*n^2) + 1 */
/* The implementation follows directly from that formula, using only integer arithmetic. */

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

seq_t a(idx_t n) {
    /* Cast the index to seq_t for arithmetic */
    seq_t nS = (seq_t)n;

    /* Compute t = 5 * n^2 */
    seq_t t = (seq_t)5 * nS * nS;

    /* m = floor_sqrt(t) = floor(n*sqrt(5)) */
    seq_t m = floor_sqrt(t);

    /* diff = (m + 1)^2 */
    seq_t diff = (m + (seq_t)1) * (m + (seq_t)1);

    /* sub = 5 * n^2 */
    seq_t sub = t; /* same as (seq_t)5 * nS * nS */

    /* Return the difference, which is always positive */
    return diff - sub;
}
