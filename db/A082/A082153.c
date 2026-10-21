/*
 * A082153  Dispersion of the complement of row 1 of A082152.
 * https://oeis.org/A082153
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 69, u16 n <= 69, u32 n <= 69, u64 n <= 69, u128 n <= 69,
 *            big any n
 * Helpers:   _row_first
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * OEIS A082153 – "Dispersion of the complement of row 1 of A082152"
 * The sequence is read by antidiagonals from a square array
 * with rows and columns indexed from 1.
 *
 * Formula:
 *   Let (i,j) be the coordinates of the element with index n
 *   (n is offset‑1 based).  
 *   Compute the diagonal number d as the smallest integer with
 *      d*(d+1)/2 >= n .
 *   Let prev = (d-1)*d/2  (number of elements before this diagonal)
 *   Then i = n - prev   (row index, 1‑based in the diagonal)
 *        j = d + 1 - i  (column index)
 *
 *   a(i,1) (the first element of each row) is given by
 *        a(1,1) = 1
 *        for i >= 2:
 *          let m = i-2
 *          let q = m / 3,     r = m % 3
 *          a(i,1) = 1 + 3*q*(q+1)/2 + (r+1)*(q+1)
 *
 *   For j > 1:
 *        a(i,j) = a(i,1) + (j-1)*i + 3*(j-1)*(j)/2
 *
 * All intermediate arithmetic is performed on type seq_t,
 * cast appropriately from idx_t, which is an unsigned
 * integer type up to uint64_t.
 *
 * The implementation is in pure C11 (also C++17 compatible),
 * compliant with the build rules for seq_t and idx_t.
 */

#include <stdint.h>
#include "../utils/_row_first.h"

seq_t a(idx_t n)
{
    if (n == 0) return (seq_t)0;  /* not part of sequence */

    /* Find diagonal d: smallest d with d*(d+1)/2 >= n */
    idx_t low  = 1;
    idx_t high = 1;
    while ((unsigned __int128)high * (high + 1) / 2 < n) high <<= 1;
    while (low < high) {
        idx_t mid = low + (high - low) / 2;
        if ((unsigned __int128)mid * (mid + 1) / 2 >= n)
            high = mid;
        else
            low = mid + 1;
    }
    idx_t d   = low;
    idx_t prev = (d - 1) * d / 2;   /* elements before diagonal d */
    idx_t i   = n - prev;           /* row index in this diagonal (1‑based) */
    idx_t j   = d + 1 - i;           /* column index (1‑based) */

    seq_t a_i1 = _row_first(i);

    /* t = j - 1 */
    idx_t t = j - 1;

    /* term1 = t * i  */
    seq_t term1 = (seq_t)t * (seq_t)i;

    /* term2 = 3 * t * (t + 1) / 2  */
    seq_t term2 = (seq_t)3 * (seq_t)t * (seq_t)(t + 1) / 2;

    return a_i1 + term1 + term2;
}
