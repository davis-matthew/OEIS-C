/*
 * A116941  Permutation of the natural numbers in conjunction with A116939 and
 *          A003056.
 * https://oeis.org/A116941
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = A074147(n+1) - 1
 * Range:     u8 n <= 248, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* A074147 is defined by the following rule:
 *   Set val = 1 for index 1.
 *   For each subsequent index i starting at 1:
 *     - If i is a triangular number T(k) = k*(k+1)/2 with k>=1:
 *           if k==1 : val += 1
 *           else    : val -= (k-1)-(k&1)
 *     - Otherwise val += 2
 * The required a(n) is val minus 1 for index n+1. */

#include <stdint.h>   /* ensures that idx_t is seen if needed */

seq_t a(idx_t n)
{
    /* Compute A074147(k) for k = n+1, then subtract 1. */
    idx_t target = n + (idx_t)1;          /* k = n+1 */

    /* Start with A074147(1) = 1. */
    seq_t val = (seq_t)1;

    /* Prepare triangular number tracking. */
    idx_t tri_k   = (idx_t)1;             /* current triangular index k */
    idx_t next_tri = (idx_t)1;            /* T(k) = k*(k+1)/2 */

    /* Iterate from index 1 up to target-1 to reach value for target. */
    for (idx_t i = (idx_t)1; i < target; ++i)
    {
        if (i == next_tri)                /* we hit a triangular index */
        {
            if (tri_k == (idx_t)1)
            {
                val += (seq_t)1;           /* special first step */
            }
            else
            {
                idx_t d = tri_k - (idx_t)1 - (tri_k & (idx_t)1);
                val -= (seq_t)d;           /* decrement by d */
            }
            ++tri_k;
            next_tri = tri_k * (tri_k + (idx_t)1) / (idx_t)2;
        }
        else                              /* normal step */
        {
            val += (seq_t)2;
        }
    }

    return val - (seq_t)1;
}
