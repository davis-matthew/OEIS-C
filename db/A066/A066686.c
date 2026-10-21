/*
 * A066686  Array T(i,j) read by antidiagonals, where T(i,j) is the
 *          concatenation of i and j (1<=i, 1<=j).
 * https://oeis.org/A066686
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, tabl, base
 * Formula:   not stated
 * Range:     u8 n <= 66, u16 n <= 66, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   digit_count, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: locate the (i,j) coordinates on the antidiagonals
   of the infinite i↦j table and return the decimal concatenation
   of i and j.  The formula for the index is
     n = (i+j-1)(i+j-2)/2 + i
   (with i,j ≥ 1).  We invert that to recover i and j. */
#include <stdint.h>
#include "../utils/digit_count.h"
#include "../utils/pow10.h"

seq_t a(idx_t n) {
    /* Find the diagonal d such that d(d-1)/2 >= n  */
    idx_t d = 2;
    idx_t tri = 1;           /* end index of diagonal d */
    while (tri < n) {
        ++d;
        tri += d - 1;        /* each new diagonal adds d-1 terms */
    }

    /* base = sum_{k=1}^{d-2} k = (d-1)(d-2)/2   */
    idx_t base = ((d - 1) * (d - 2)) / 2;
    idx_t i = n - base;       /* 1 <= i <= d-1 */
    idx_t j = d - i;          /* 1 <= j <= d-1 */

    /* Build 10^{digits(j)} */
    idx_t digits_j = digit_count(j);
    seq_t pow10_digits = pow10(digits_j);

    /* Concatenate i and j:  i * 10^{digits(j)} + j */
    return (seq_t)i * pow10_digits + (seq_t)j;
}
