/*
 * A034105  Numbers n such that fractional part of square root of n starts
 *          with digit 9.
 * https://oeis.org/A034105
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, base
 * Formula:   a(n) is the nth integer n such that the fractional part of
 *            sqrt(n) starts with digit 9 i.e. n lies in an interval
 *            [ceil((k+0.9)^2), (k+1)^2−1] for some integer k. This code scans
 *            these intervals in increasing order of k and returns the nth
 *            term
 * Range:     u8 n <= 18, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    idx_t count = 0;              /* how many numbers have been seen so far */
    for (idx_t k = 0;; ++k) {     /* iterate over k starting from 0   */
        /* Compute lower bound LB = ceil((k+0.9)^2) */
        unsigned long long t   = 10ULL * k + 9ULL;          /* 10*k+9 */
        unsigned long long sq  = t * t;                    /* (10k+9)^2 */
        unsigned long long lb  = (sq + 99ULL) / 100ULL;    /* ceil division by 100 */

        /* Compute upper bound UB = (k+1)^2 - 1 */
        unsigned long long ub  = (k + 1) * (k + 1) - 1ULL;

        if (lb > ub)                /* empty interval, skip */
            continue;

        for (unsigned long long val = lb; val <= ub; ++val) {
            ++count;
            if (count == n)              /* found the nth term */
                return (seq_t)val;
        }
    }
    /* If n is 0 the loop would never terminate; however, the sequence is
       defined for n >= 1 only.  Returning 0 is a safe no‑op for out‑of‑range. */
    return (seq_t)0;
}
