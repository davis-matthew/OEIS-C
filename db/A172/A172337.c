/*
 * A172337  Floor(n*(sqrt(11)+sqrt(7))).
 * https://oeis.org/A172337
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, less, easy
 * Formula:   a(n) = floor(n*(sqrt(11)+sqrt(7)))
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 101, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;

    /* Convert index to seq_t for arithmetic */
    seq_t nn   = (seq_t)n;
    seq_t nSq  = nn * nn;

    /* Search interval: 5*n <= a(n) < 6*n */
    seq_t low  = nn * 5ULL;   /* 5*n */
    seq_t high = nn * 6ULL;   /* 6*n */

    while (low < high)
    {
        /* Upper mid to avoid infinite loop */
        seq_t mid = (low + high + 1) >> 1;

        /* Test whether mid <= n * (sqrt(11)+sqrt(7)) */
        seq_t midSq  = mid * mid;
        seq_t prod18 = nSq * 18ULL;

        if (midSq < prod18)
        {
            /* mid <= sqrt(18)*n < n*(sqrt(11)+sqrt(7)) */
            low = mid;
            continue;
        }

        /* midSq >= 18*n^2, use the exact inequality:
           mid <= n*C  iff (midSq - 18*n^2)^2 <= 4*77*n^4
        */
        seq_t X      = midSq - prod18;          /* non‑negative */
        seq_t lhs    = X * X;                   /* (midSq-18*nSq)^2 */
        seq_t nSqSq  = nSq * nSq;               /* n^4          */
        seq_t rhs    = nSqSq * 308ULL;          /* 4*77*n^4     */

        if (lhs <= rhs)
            low = mid;     /* condition holds, try larger mid */
        else
            high = mid - 1;
    }

    return low;
}
