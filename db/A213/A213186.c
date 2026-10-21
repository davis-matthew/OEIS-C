/*
 * A213186  For the Collatz (3x+1) iteration starting at n, number of integers
 *          k such that T(n,k) = T(n-1,k) where T(n,k) is the k-th number of
 *          the trajectory.
 * https://oeis.org/A213186
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = #indices k where the Collatz trajectories of n and n-1
 *            agree at the same step. The trajectory is defined by T(x,0)=x,
 *            T(x,k+1)=next(T(x,k)) where next(y) = y/2 if y even, 3*y+1 if y
 *            odd, and stops when y==1. The sequences stop at 1; only common
 *            values up to that step are counted
 * Range:     u8 n <= 54, u16 n <= 7086, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdbool.h>

static inline seq_t collatz_next(seq_t v)
{
    return (v & 1) ? (v * 3u + 1u) : (v >> 1u);
}

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;   /* out of range, just guard */
    if (n == 1) return (seq_t)0;   /* a(1)=0 by definition */

    seq_t v1 = (seq_t)n;           /* trajectory of n      */
    seq_t v2 = (seq_t)(n - 1);     /* trajectory of n-1    */
    seq_t count = (seq_t)0;

    for (;;) {
        /* Count current position if both are equal */
        if (v1 == v2) count++;

        /* If either has reached 1, stop after this comparison */
        if (v1 == (seq_t)1 || v2 == (seq_t)1) break;

        /* Advance both sequences */
        v1 = collatz_next(v1);
        v2 = collatz_next(v2);
    }
    return count;
}
