/*
 * A064882  Eisenstein array Ei(2,1).
 * https://oeis.org/A064882
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy, tabf
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   getA
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Recurrence: a(1,0)=2, a(1,1)=1;
   For n>1:
     if m is even: a(n,m) = a(n-1, m/2);
     else:          a(n,m) = a(n-1, (m-1)/2) + a(n-1, (m+1)/2);
   The flattened sequence a(n) is obtained by concatenating the rows
   for n=1,2,3,…  (row n contains 2^(n-1)+1 terms indexed 0 … 2^(n-1)). */

#include <stdint.h>
#include "../utils/getA.h"

seq_t a(idx_t n)
{
    if (n == 0)  /* undefined, but safe to return 0 */
        return (seq_t)0;

    idx_t sum = 0;   /* number of terms before current row */
    idx_t r   = 1;   /* current row number */

    /* find the row that contains the n-th term   */
    while (sum + (((idx_t)1) << (r - 1)) + 1 < n) {
        sum += (((idx_t)1) << (r - 1)) + 1;
        ++r;
    }

    idx_t m = n - sum - 1;  /* column index within the found row */
    return getA(r, m);
}
