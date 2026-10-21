/*
 * A214270  Number of compositions of n where the difference between largest
 *          and smallest parts equals 1 and adjacent parts are unequal.
 * https://oeis.org/A214270
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 10000, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Definition: a(n) equals the number of compositions of n in which
 * the largest and smallest parts differ by 1 and any two adjacent
 * parts are unequal.  The compositions must use two consecutive
 * integers k and k+1 and alternate them.  This routine counts all
 * such compositions for any n ≥ 1. */

/* The calculation is performed by enumerating the possible
 * starting part a (the smaller of the two consecutive integers)
 * and the number of full two‑part blocks t.  Three cases arise:
 *
 *   (1) Even length (m = 2t):   n = t · (2a+1)      –  two alternations,
 *                               counted twice (start with a or start with a+1).
 *
 *   (2) Odd length, start with a (m = 2t+1 ≥ 3):
 *                       n = t · (2a+1) + a.
 *
 *   (3) Odd length, start with a+1 (m = 2t+1 ≥ 3):
 *                       n = t · (2a+1) + (a+1).
 *
 * All cases are tested for integer solutions with a ≥ 1 and t ≥ 1
 * (t = 0 corresponds to a single part and is excluded automatically).
 *
 * Since the algorithm runs in O(√n + n) time it easily satisfies
 * the performance requirement for n up to 10 000. */

#include <stdbool.h>

seq_t a(idx_t n) {
    if (n < 3)           /* a(1)=a(2)=0 by definition */
        return 0;

    seq_t res = 0;

    /* Case (1): Even length compositions, counted twice. */
    for (idx_t d = 1; d * d <= n; ++d) {
        if (n % d == 0) {
            /* t = d */
            idx_t odd1 = n / d;           /* 2a+1 */
            if ((odd1 & 1) && odd1 >= 3)
                res += (seq_t)2;

            /* t = n/d  (if different) */
            idx_t t2 = n / d;
            if (t2 != d) {
                idx_t odd2 = n / t2;      /* = d */
                if ((odd2 & 1) && odd2 >= 3)
                    res += (seq_t)2;
            }
        }
    }

    /* Case (2): Odd length, start with the smaller part. */
    for (idx_t t = 1; t < n; ++t) {
        idx_t denom = 2 * t + 1;  /* 2t+1 > 0 */
        idx_t numer = n - t;      /* n - t  > 0 for t < n */
        if (numer % denom == 0) {
            idx_t a_val = numer / denom;
            if (a_val >= 1)
                res += (seq_t)1;
        }

        /* Case (3): Odd length, start with the larger part. */
        idx_t numerC = n - t - 1;  /* n - t - 1  > 0 for t < n-1 */
        if (numerC > 0 && numerC % denom == 0) {
            idx_t a_val = numerC / denom;
            if (a_val >= 1)
                res += (seq_t)1;
        }
    }

    return res;
}
