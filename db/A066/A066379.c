/*
 * A066379  Sum of the first n numbers m such that floor(log(m)) divides m.
 * https://oeis.org/A066379
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 175, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A066379 – Sum of the first n integers m such that ⌊log(m)⌋ divides m
 *
 *  a(1) = 3
 *  a(n) = sum of the first n integers m where floor(log(m)) | m
 *
 *  The algorithm iterates over the binary intervals for each integer k = floor(log(m)),
 *  which are:  e^k ≤ m < e^(k+1).  For every k the qualifying numbers are the multiples
 *  of k in that interval.  The sum of such multiples in an interval is an arithmetic
 *  progression, which we can add in O(1) time.  The process stops as soon as the
 *  required n numbers are collected.  This runs in O(n) time and uses only
 *  standard integer arithmetic.
 */

#include <math.h>

seq_t a(idx_t n)
{
    if (n == (idx_t)0)
        return 0ULL;               /* safety for out-of-range indices */

    seq_t total = 0ULL;
    idx_t remaining = n;              /* numbers still needed */

    for (idx_t k = 1; remaining > 0; ++k) {
        /* bounds of the interval where floor(log(m)) == k */
        double low  = exp((double)k);
        double high = exp((double)(k + 1));

        /* small epsilon prevents rounding errors at the boundaries */
        const double eps = 1e-12;

        idx_t L = (idx_t)ceil(low  - eps);   /* smallest integer >= exp(k) */
        idx_t R = (idx_t)floor(high - eps);  /* largest integer <  exp(k+1) */

        if (L > R)
            continue;               /* empty interval – skip */

        /* first multiple of k not less than L */
        idx_t m0 = ((L + k - 1) / k) * k;
        if (m0 > R)
            continue;               /* no qualifying number in this interval */

        idx_t cnt_k = (R - m0) / k + 1;   /* count of multiples of k in [L,R] */

        if (cnt_k <= remaining) {
            /* full interval contributes */
            seq_t m0_t = (seq_t)m0;
            seq_t k_t  = (seq_t)k;
            seq_t last = m0_t + (seq_t)(cnt_k - 1) * k_t;
            total += (seq_t)cnt_k * (m0_t + last) / 2;
            remaining -= cnt_k;
        } else {
            /* partial interval – only the first 'remaining' multiples */
            idx_t rem = remaining;
            seq_t m0_t = (seq_t)m0;
            seq_t k_t  = (seq_t)k;
            seq_t last = m0_t + (seq_t)(rem - 1) * k_t;
            total += (seq_t)rem * (m0_t + last) / 2;
            return total;
        }
    }

    return total;   /* should never reach here for positive n */
}
