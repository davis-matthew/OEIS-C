/*
 * A103323  Square array T(n,k) read by antidiagonals: powers of Fibonacci
 *          numbers.
 * https://oeis.org/A103323
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) is the n-th term of the array T(i,j)=Fib(j)^i read by *
 *            antidiagonals with the indices on each diagonal taken in
 *            decreasing * order of i (i.e. n). That is, for each d=i+j we
 *            list the pairs * (i,j) with i descending from d-1 to 1. The
 *            first few terms are * 1,1,1,1,1,2,1,1,4,3,1,1,8,9,5,1,1,16...
 * Range:     u8 n <= 32, u16 n <= 59, u32 n <= 111, u64 n <= 200,
 *            u128 n <= 391, big any n
 * Helpers:   fib_u64, seq_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/fib_u64.h"
#include "../utils/seq_pow.h"

seq_t a(idx_t n)
{
    /* Find diagonal d such that d*(d-1)/2 >= n.
       d starts at 2 because the first diagonal has d=2 (i=1, j=1). */
    idx_t d   = 2;
    idx_t sum = 1;                /* total number of elements up to diagonal d   */
    while (n > sum) {
        ++d;
        sum += d-1;               /* each new diagonal adds (d-1) elements     */
    }

    /* sum now equals d*(d-1)/2.  The sum of elements before this diagonal is
       sum - (d-1).  Compute the offset within this diagonal (0‑based). */
    idx_t prev_sum = sum - (d-1);
    idx_t offset   = n - prev_sum - 1;  /* 0 … d-2 */

    /* In the reversed order on the diagonal, i = d-1 - offset,
       j = d - i. */
    idx_t i = (d-1) - offset;
    idx_t j = d - i;

    /* Compute Fib(j)^i using the provided utilities. */
    seq_t fib_j = fib_u64(j);
    return seq_pow(fib_j, i);
}
