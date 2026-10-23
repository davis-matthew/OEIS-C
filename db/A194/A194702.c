/*
 * A194702  Triangle read by rows: T(k,m) = number of occurrences of k in the
 *          last section of the set of partitions of (2 + m).
 * https://oeis.org/A194702
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 91, u16 n <= 91, u32 n <= 91, u64 n <= 91, u128 n <= 91,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
   OEIS A194702 - Triangle read by rows
   T(k,m) = number of occurrences of k in the last section of the set of
   partitions of (2 + m).  The sequence a(n) is obtained by flattening
   the triangle row-wise: for each m = 0,1,2,... and k = 1..(m+1)
   we output T(k,m).  The mapping from index n (n ≥ 1) to (m,k) is
   n = m*(m+1)/2 + k, 1 ≤ k ≤ m+1.

   The triangle satisfies:
     * for m = 0   : T(1,0) = 2
     * for m = 1   : T(1,1) = 0, T(2,1) = 2
     * for m ≥ 2   : T(k,m) = 1 if k = m-1 or k = m+1, otherwise 0.
   Thus the sequence values are only 0, 1 or 2.
*/

#include <stddef.h>
#include <stdbool.h>

seq_t a(idx_t n)
{
    if (n < 1)      /* undefined for n < 1; return 0 for safety */
        return 0;

    /* Find m such that m*(m+1)/2 < n ≤ (m+1)*(m+2)/2.
       Use a simple increasing loop; m ≤ sqrt(2*n) ≈ 141 for n ≤ 10000. */
    idx_t m = 0;
    idx_t t = 0;                     /* t = m*(m+1)/2 */
    while (t + m + 1 < n) {
        ++m;
        t += m;
    }
    /* now t = m*(m+1)/2  and  k = n - t (1 ≤ k ≤ m+1) */
    idx_t k = n - t;

    seq_t result = 0;

    if (m == 0) {
        /* Only k==1 is possible */
        result = 2;                   /* T(1,0) */
    } else if (m == 1) {
        /* k == 2 gives value 2, all others 0 */
        if (k == 2)
            result = 2;
        else
            result = 0;
    } else {
        /* m >= 2 : T(k,m) = 1 if k == m-1 or k == m+1, else 0 */
        if (k == m - 1 || k == m + 1)
            result = 1;
        else
            result = 0;
    }

    return result;
}
