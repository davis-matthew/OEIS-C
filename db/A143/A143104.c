/*
 * A143104  Infinite Redheffer matrix read by upwards antidiagonals.
 * https://oeis.org/A143104
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 1 if j==1 or i divides j, where (i , j) are the cell *
 *            coordinates on the d‑th upwards antidiagonal of the Redheffer
 *            matrix: * i + j = d + 1 and the diagonal elements are listed
 *            from * (i = d , j = 1) up to (i = 1 , j = d). * a(n) is 0
 *            otherwise
 * Range:     u8 n <= 5050, u16 n <= 5050, u32 n <= 5050, u64 n <= 5050,
 *            u128 n <= 5050, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    if (n == 0)     /* undefined for n<1, but avoid UB */
        return (seq_t)0;

    /* Find diagonal d such that cumulative entries up to d >= n.
       cumulative = d(d+1)/2.  Iterate because d is at most ~sqrt(2n). */
    idx_t d = 1;
    idx_t cumulative = 1;           /* T_1 */
    while (n > cumulative) {
        ++d;
        cumulative += d;            /* cumulative = T_d */
    }

    idx_t prev_cumulative = cumulative - d;  /* T_{d-1} */
    idx_t offset   = n - prev_cumulative - 1;/* 0‑based index within diagonal */

    idx_t i = d - offset;     /* row index (1‑based)   */
    idx_t j = offset + 1;     /* column index (1‑based) */

    /* Cell contributes 1 if j==1 or i divides j. */
    seq_t result = (seq_t)((j == 1) || (j % i == 0));
    return result;
}
