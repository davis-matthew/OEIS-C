/*
 * A202452  Lower triangular Fibonacci matrix, by SW antidiagonals.
 * https://oeis.org/A202452
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 78, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   fib_if_nonzero
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) is obtained by reading the lower‑triangular Fibonacci matrix
   by southwest (SW) antidiagonals.  
   For the n-th term let
         d  = smallest integer with d(d+1)/2 ≥ n             (diagonal index)
         p  = n – d(d-1)/2                                   (position in the diagonal, 1…d)
         i  = d – p + 1
         j  = p
   If i < j the matrix entry is 0, otherwise the value
   is Fibonacci(i−j+1).  Fibonacci(1)=1, Fibonacci(2)=1.  */

#include <stdbool.h>
#include "../utils/fib_if_nonzero.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d: d(d+1)/2 ≥ n */
    idx_t d = 1;
    while ((d * (d + 1)) / 2 < n) {
        ++d;
    }

    /* Position p within that diagonal */
    idx_t prev = (d - 1) * d / 2;
    idx_t p = n - prev;          /* 1 ≤ p ≤ d */

    /* Coordinates of the matrix entry */
    idx_t i = d - p + 1;         /* row index   */
    idx_t j = p;                 /* column index */

    /* If the entry lies above the diagonal, it is zero */
    if (i < j) {
        return (seq_t)0;
    }

    /* Fibonacci index is i−j+1 */
    idx_t k = i - j + 1;   /* k ≥ 1 */

    return fib_if_nonzero(k);
}
