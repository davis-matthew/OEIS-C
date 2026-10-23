/*
 * A189459  a(n) = n + [n*s/r] + [n*t/r]; r=1, s=1+sqrt(2), t=1/2+sqrt(2).
 * https://oeis.org/A189459
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = n + [n*(1+sqrt(2))] + [n*(1/2+sqrt(2))] where [x] denotes
 *            the floor of x and the brackets are integers. The implementation
 *            uses integer arithmetic only, relying on the provided floor_sqrt
 *            function to compute floor(n*sqrt(2))
 * Range:     u8 n <= 5, u16 n <= 1915, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"   /* provides seq_t floor_sqrt(seq_t x) */

seq_t a(idx_t n)
{
    /* Convert the index to seq_t once for repeated use. */
    seq_t ns = (seq_t)n;

    /* b = floor(n*sqrt(2)) computed via integer square root. */
    seq_t two_n_sq = (seq_t)2 * ns * ns;            /* 2 * n^2   */
    seq_t b = floor_sqrt(two_n_sq);                /* floor(sqrt(2)*n) */

    /* floor(n/2) */
    seq_t half = ns / 2;

    /* Determine if the fractional part of n*sqrt(2) is >= 0.5
       (needed only when n is odd).  We use the inequality
           (b + 0.5)^2  <=  2*n^2   ?
       to avoid floating-point calculations. */
    bool extra = false;
    if ((n & 1) != 0) {          /* odd n */
        seq_t left  = (seq_t)8 * ns * ns;          /* 8*n^2        */
        seq_t right = (seq_t)4 * b * b + (seq_t)4 * b + (seq_t)1;  /* 4*b^2+4*b+1 */
        if (left >= right)        /* fractional part >= 0.5 */
            extra = true;         /* will add one more to floor(n*t) */
    }

    /* a(n) = 2*n + 2*b + floor(n/2) + (extra ? 1 : 0) */
    seq_t ans = (seq_t)2 * ns + (seq_t)2 * b + half + (extra ? (seq_t)1 : (seq_t)0);
    return ans;
}
