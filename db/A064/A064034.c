/*
 * A064034  2-dimensional table T(i, j) defined for any integers i and j, read
 *          by antidiagonals in the southeast quadrant. T(i, j) gives the
 *          "Fibonacci depth" of (i, j): form the Fibonacci sequence starting
 *          with i, j: w(0) = i, w(1) = j, w(n) = w(n-1) + w(n-2). It can be
 *          shown that for all but finitely many...
 * https://oeis.org/A064034
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, nice
 * Formula:   not stated
 * Range:     u8 n <= 104, u16 n <= 104, u32 n <= 104, u64 n <= 104,
 *            u128 n <= 104, big any n
 * Helpers:   antidiagonal_coords
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute OEIS A064034.  The sequence a(n) is the Fibonacci depth
   T(i,-j) where (i,j) are the antidiagonal coordinates of n
   (with i,j ≥ 0).  The depth is the smallest t such that
   w(t) and w(t+1) have the same sign for the recurrence
   w(0)=i, w(1)=-j, w(k)=w(k-1)+w(k-2).  */
#include <stdint.h>
#include <stddef.h>
#include <stdbool.h>
#include "../utils/antidiagonal_coords.h"

seq_t a(idx_t n)
{
    idx_t i, j;
    antidiagonal_coords(n, &i, &j);          /* i,j >= 0 */

    long long a0 = (long long)i;            /* w(0) */
    long long b0 = -(long long)j;           /* w(1) = -j */

    int step = 0;
    for (;; ++step) {
        int s1 = a0 > 0 ? 1 : (a0 < 0 ? -1 : 0);
        int s2 = b0 > 0 ? 1 : (b0 < 0 ? -1 : 0);
        if (s1 == s2)            /* same sign → depth found */
            return (seq_t)step;

        long long next = a0 + b0; /* w(k+1) = w(k-1) + w(k)   */
        a0 = b0;
        b0 = next;
    }
}
