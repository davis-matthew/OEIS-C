/*
 * A032347  Inverse binomial transform of A032346.
 * https://oeis.org/A032347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy, eigen
 * Formula:   a(0)=1, a(1)=0. For n >= 2 a(n) = sum_{j=0}^{n-1} binomial(n-1,
 *            j) * a(j)
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 16, u64 n <= 22, u128 n <= 22,
 *            big any n
 * Helpers:   alloc_seq, free_seq
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* This implements the recurrence directly, using a temporary array
   for the values up to n.  It works for all lengths that fit in
   the int‑type idx_t and for any seq_t that supports the usual
   arithmetic operators and the utility functions alloc_seq/free_seq. */

#include <stdlib.h>
#include "../utils/alloc_seq.h"
#include "../utils/free_seq.h"

seq_t a(idx_t n)
{
    /* Handle the two trivial cases first.  No memory allocation
       is needed for these. */
    if (n == 0) return (seq_t)1;
    if (n == 1) return (seq_t)0;

    /* Allocate an array of seq_t of size n+1.  The helper functions
       alloc_seq/free_seq are designed to work with both builtin
       integer types and user‑defined arbitrary‑precision classes. */
    seq_t *val = alloc_seq(n + 1);

    /* Base values. */
    val[0] = (seq_t)1;
    val[1] = (seq_t)0;

    /* Compute the values 2 .. n sequentially. */
    for (idx_t i = 2; i <= n; ++i)
    {
        seq_t sum = val[0];            /* binomial(n-1,0)*a(0) = 1*a(0)  */
        seq_t binom = (seq_t)1;        /* current binomial coefficient    */

        /* Compute binomial coefficients iteratively:
           binom_{j} = binom_{j-1} * (i - j) / j   */
        for (idx_t j = 1; j < i; ++j)
        {
            binom = binom * (seq_t)(i - j);   /* multiply by (i-j)  */
            binom = binom / (seq_t)j;        /* divide by j (exact)*/
            sum += binom * val[j];
        }
        val[i] = sum;
    }

    seq_t result = val[n];
    free_seq(val);                     /* release the temporary array */
    return result;
}
