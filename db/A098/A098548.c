/*
 * A098548  a(n) = n if n <= 3, otherwise the smallest number > a(n-1) having
 *          at least one common factor with a(n-2) but none with a(n-1).
 * https://oeis.org/A098548
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 68, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   gcd
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1)=1, a(2)=2, a(3)=3.
   For n>3, a(n) = smallest m > a(n-1) with gcd(m,a(n-2))>1
   and gcd(m,a(n-1))==0. */

#include "../utils/gcd.h"      /* gcd(seq_t, seq_t) */

seq_t a(idx_t n)
{
    /* Handle the initial terms directly. */
    if (n <= 0) {
        /* Undefined for n < 1 – return 0 as a safe default. */
        return (seq_t)0;
    }
    if (n <= 3)
        return (seq_t)n;

    /* Variables to hold the two previous terms:
       prev1 = a(k-1), prev2 = a(k-2) while computing a(k).  */
    seq_t prev2 = (seq_t)2;           /* a(2) */
    seq_t prev1 = (seq_t)3;           /* a(3) */
    seq_t cur   = (seq_t)0;           /* will hold a(k) */

    for (idx_t k = 4; k <= n; ++k) {
        seq_t candidate = prev1 + (seq_t)1;
        /* Find the smallest candidate > prev1 satisfying the
           coprimality conditions. */
        for (;;) {
            if (gcd(candidate, prev2) != (seq_t)1 &&
                gcd(candidate, prev1) == (seq_t)1) {
                cur = candidate;
                break;
            }
            ++candidate;
        }
        /* Advance the sliding window of last two terms. */
        prev2 = prev1;
        prev1 = cur;
    }

    return prev1; /* a(n) is the last computed term a(n). */
}
