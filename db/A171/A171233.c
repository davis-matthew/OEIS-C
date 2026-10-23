/*
 * A171233  Array, T(n,k) = 2*(n/k), if n mod k = 0; otherwise, T(n,k) = 1.
 *          Read by antidiagonals.
 * https://oeis.org/A171233
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  cofr, nonn, tabl
 * Formula:   a(n) = T(i,j) read by antidiagonals, where T(n,k) = 2*(n/k) if n
 *            mod k = 0 else 1. The array is indexed by i (row) and j (col),
 *            both >=1. The sequence enumerates the array along antidiagonals
 *            with fixed i+j, starting at i=j=1 and moving down the first item
 *            of each diagonal (i = sum-1) to the last...
 * Range:     u8 n <= 7, u16 n <= 99, u32 n <= 99, u64 n <= 99, u128 n <= 99,
 *            big any n
 * Helpers:   floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/floor_sqrt.h"

seq_t a(idx_t n)
{
    /* Find diagonal sum s such that (s-1)*s/2 >= n. */
    seq_t t   = (seq_t)1 + (seq_t)8 * (seq_t)n;          /* 1 + 8*n */
    seq_t s1  = floor_sqrt(t);                           /* floor(sqrt) */
    idx_t s   = ((idx_t)s1 + 1) >> 1;                    /* ceil((1+sqrt)/2)  */

    /* Adjust s to satisfy the inequality.  The loop runs at most a few times. */
    while ( (((seq_t)(s - 1) * (seq_t)s) / 2) < n ) ++s;
    while ( s > 2 &&
            (((seq_t)(s - 2) * (seq_t)(s - 1)) / 2) >= n ) --s;

    /* Count of all elements before diagonal s. */
    seq_t prev = (((seq_t)(s - 2) * (seq_t)(s - 1)) / 2);

    /* Position within the diagonal (1‑based). */
    idx_t k   = (idx_t)(n - prev);

    /* Coordinates (i,j) on the array. */
    idx_t i = s - k;
    idx_t j = s - i;

    /* Compute T(i,j). */
    seq_t i_seq = (seq_t)i;
    seq_t j_seq = (seq_t)j;
    seq_t ans = ( i_seq % j_seq == 0 ) ? 2 * (i_seq / j_seq) : 1;

    return ans;
}
