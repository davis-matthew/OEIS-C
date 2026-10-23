/*
 * A121487  Triangle read by rows: T(n,k) is the number of nondecreasing Dyck
 *          paths of semilength n and having abscissa of first return equal to
 *          2k (1<=k<=n). A nondecreasing Dyck path is a Dyck path for which
 *          the sequence of the altitudes of the valleys is nondecreasing.
 * https://oeis.org/A121487
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 36, u16 n <= 91, u32 n <= 325, u64 n <= 1176,
 *            u128 n <= 1275, big any n
 * Helpers:   fib
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A121487
   Derived from the triangle T(n,k) read by rows.
   For n>=1, let r be the unique integer with r(r+1)/2 >= n > (r-1)r/2
   (row r).  Let k = n - (r-1)r/2 (column, 1 <= k <= r).  Then

   T(n,k) = Fibonacci(2r-2k-1)   if k < r
            Fibonacci(2r-3)       if k = r

   The sequence a(n) is the nth element when all rows are concatenated.
   (Here Fibonacci(1)=1, Fibonacci(2)=1, ...).
   The function uses the provided fib() helper.
*/

#include <stdint.h>
#include "../utils/fib.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;      /* undefined for offset 1, but harmless   */
    if (n == 1) return (seq_t)1;      /* row 1 has single entry 1                 */

    /* Find row r such that r(r+1)/2 >= n.  r grows slowly (≈√(2n)).    */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n) {
        r++;
    }

    /* Column index k (1‑based).  k = n - sum of previous rows.          */
    idx_t prev_rows_sum = (r - 1) * r / 2;
    idx_t k = n - prev_rows_sum;      /* 1 <= k <= r */

    /* Compute the Fibonacci index.                                   */
    idx_t fib_index;
    if (k == r) {
        fib_index = 2 * r - 3;         /* T(r,r) case */
    } else {
        fib_index = 2 * r - 2 * k - 1; /* T(r,k), k<r  */
    }

    /* fib() returns seq_t; fib_index fits into idx_t.                 */
    return fib(fib_index);
}
