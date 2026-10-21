/*
 * A204022  Symmetric matrix based on f(i,j) = max(2i-1, 2j-1), by
 *          antidiagonals.
 * https://oeis.org/A204022
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2*A051125(n) - 1
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth antidiagonal element of the symmetric matrix
   f(i,j) = max(2i-1, 2j-1).  The sequence starts at n=1. */
#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0) /* Outside the offset, return 0 (unused) */
        return 0;

    /* Find the diagonal number t (number of elements on that diagonal) */
    idx_t t  = 0;
    idx_t cum = 0;
    while (cum < n) {
        ++t;
        cum += t;
    }

    /* cum now equals T_t = t(t+1)/2, the total number of terms
       up to and including diagonal t.  The previous total is
       T_{t-1} = cum - t.  */
    idx_t prev = cum - t;

    /* Position k within the t‑element diagonal (1‑based) */
    idx_t k = n - prev;

    /* Coordinates on the diagonal */
    idx_t i = k;
    idx_t j = t + 1 - k;

    /* The matrix entry is the larger odd number from the two indices.
       Compute it using the integer type seq_t to allow arbitrary-precision. */
    if (i >= j)
        return (seq_t)(2 * i - 1);
    else
        return (seq_t)(2 * j - 1);
}
