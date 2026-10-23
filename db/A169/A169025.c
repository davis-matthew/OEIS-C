/*
 * A169025  Number of reduced words of length n in Coxeter group on 12
 *          generators S_i with relations (S_i)^2 = (S_i S_j)^24 = I.
 * https://oeis.org/A169025
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 9, u64 n <= 17, u128 n <= 17,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Sequence A169025:
 *
 *  a(n) = 10*Σ_{k=1}^{23} a(n-k) – 55*a(n-24) + c(n)
 *
 *  where
 *      c(0) = 1,　c(24) = 1,
 *      c(1…23) = 2,
 *      c(n>24) = 0
 *
 *  This follows from the generating function
 *  (t^24 + 2t^23 + … + 2t + 1) /
 *  (55t^24 – 10t^23 – … – 10t + 1).
 *
 *  The implementation keeps a circular buffer of the most recent
 *  24 values and a running sum of the last 23 values in order
 *  to evaluate the recurrence in O(1) per term.
 *
 *  The function is memoised; repeated calls with larger indices
 *  extend the pre‑computed range.
 */

#include <stddef.h>     /* for size_t */

seq_t a(idx_t n)
{
    /* 24-term recurrence */
    enum { R = 24 };
    /* static buffer of the last 24 values */
    static seq_t buf[R]   = { 0 };
    /* number of values already computed (next index to compute) */
    static idx_t computed = 0;
    /* running sum of the last up to 23 values (a_{i-1} … a_{i-23}) */
    static seq_t sumLast = 0;

    /* extend the cache up to n */
    while (computed <= n) {
        idx_t i = computed;                     /* index being filled */

        /* coefficient c(i) */
        seq_t c = 0;
        if (i == 0 || i == 24)          c = 1;
        else if (i <= 23)                c = 2;
        /* else c stays 0 for i > 24 */

        /* term that is 24 positions behind (a_{i-24}) */
        seq_t val24 = 0;
        if (i >= 24) {
            /* buf[i % R] currently holds a_{i-24} before overwriting */
            val24 = buf[(size_t)(i % R)];
        }

        /* recurrence */
        seq_t ai = 10 * sumLast - 55 * val24 + c;

        /* store the new value */
        buf[(size_t)(i % R)] = ai;

        /* update sumLast for the next iteration */
        sumLast += ai;
        if (i >= 23) {
            /* subtract a_{i-23} to keep the window size at 23 */
            seq_t sub = buf[(size_t)((i - 23) % R)];
            sumLast -= sub;
        }

        ++computed;
    }

    return buf[(size_t)(n % R)];
}
