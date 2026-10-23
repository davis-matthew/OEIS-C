/*
 * A014405  Number of arithmetic progressions of 3 or more positive integers,
 *          strictly increasing with sum n.
 * https://oeis.org/A014405
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = |{ (a,d,k) : k ≥ 3, a ≥ 1, d ≥ 1, k·a + d·k(k−1)/2 = n }|
 *            – computed by brute‑force enumeration over the possible lengths
 *            k and common differences d
 * Range:     u8 n <= 419, u16 n <= 9560, u32 n <= 9560, u64 n <= 9560,
 *            u128 n <= 9560, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#pragma once
#include <stdint.h>

seq_t a(idx_t n)
{
    /* The smallest sum of a 3‑term arithmetic progression is
       1 + 2 + 3 = 6.  For n < 6 there is no progression. */
    if (n < 6)
        return 0;

    seq_t result = 0;

    /* k = length of the progression, starting from 3 */
    idx_t k   = 3;
    idx_t T   = 3;   /* T = k(k−1)/2 for current k */

    for (;; ++k) {
        /* The minimal possible sum for this length is T + k.
           If that exceeds n we can stop.  The subtraction is safe
           because n ≥ k for all useful iterates. */
        if (T > n - k)
            break;

        /* For a given k the common difference d must satisfy
           d·T < n  →  d ≤ (n−1)/T.  This guarantees that the
           remainder rem = n − d·T stays positive. */
        idx_t max_d = (n - 1) / T;
        for (idx_t d = 1; d <= max_d; ++d) {
            idx_t rem = n - d * T;             /* remainder after removing d·T */
            if (rem % k == 0) {
                idx_t a0 = rem / k;            /* first term of the progression */
                if (a0 >= 1)
                    result += (seq_t)1;
            }
        }

        /* Prepare T for the next value of k:
           T_next = (k+1)k/2 = T + k.                 */
        T += k;
    }

    return result;
}
