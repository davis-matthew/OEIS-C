/*
 * A213558  Rectangular array: (row n) = b**c, where b(h) = h^3, c(h) =
 *          (n-1+h)^3, n>=1, h>=1, and ** = convolution.
 * https://oeis.org/A213558
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(x,y) where T(x,y)=sum_{i=1}^{y} i^3*(x+y-i)^3 (x,y) are
 *            obtained by reading the rectangular array T(n,k) (convolution of
 *            h^3 and (n-1+h)^3) along falling antidiagonals. Initial terms:
 *            a(1)=1, a(2)=16, a(3)=8, a(4)=118,…
 * Range:     u8 n <= 6, u16 n <= 36, u32 n <= 210, u64 n <= 210,
 *            u128 n <= 210, big any n
 * Helpers:   d_mul
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/d_mul.h"

/* Return a(n) as defined by the OEIS sequence A213558.  */
seq_t a(idx_t n)
{
    /* Find the diagonal d such that d(d-1)/2 >= n.  */
    idx_t d = 2;
    while (1)
    {
        /* Use 128‑bit to avoid overflow for very large n. */
        unsigned __int128 val = (unsigned __int128)d * (d - 1) / 2;
        if (val >= n)
            break;
        d++;
    }

    /* Previous sum of entries before diagonal d:
       prev = (d-1)(d-2)/2.  */
    idx_t prev = (d - 1) * (d - 2) / 2;

    /* Offset within this diagonal (0‑based). */
    idx_t offset = n - prev - 1;

    /* Coordinates (x,y) on the array:
       x = offset+1   (row index)
       y = d - x      (column index) */
    idx_t x = offset + 1;
    idx_t y = d - x;

    /* Compute T(x,y) = sum_{i=1}^{y} i^3 * (x + y - i)^3.  */
    seq_t sum = 0;
    for (idx_t i = 1; i <= y; ++i)
    {
        seq_t ii = (seq_t)i;
        seq_t ii3 = ii * ii * ii;                /* i^3   */
        seq_t j = x + y - i;                     /* x + y - i */
        seq_t jj = (seq_t)j;
        seq_t jj3 = jj * jj * jj;                /* (x+y-i)^3 */
        sum += ii3 * jj3;
    }

    return sum;
}
