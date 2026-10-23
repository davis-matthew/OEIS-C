/*
 * A139365  Array of digit sums of factorial representation of numbers
 *          0,1,...,n!-1 for n >= 1.
 * https://oeis.org/A139365
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, tabf
 * Formula:   a(n) = sum(facrep(r,m)[r-j],j=1..r) where m = n - Σ_{k=0}^{r-1}
 *            k!, r = min{r : n < Σ_{k=0}^{r} k!} (with 0! taken as 1)
 * Range:     u8 n <= 9424, u16 n <= 9424, u32 n <= 9424, u64 n <= 9424,
 *            u128 n <= 9424, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include <stdint.h>

/* Compute a(n): digit sum of the factorial representation of the
   position within its row, where rows are indexed from 0, each row r
   containing r! terms (m = 0 .. r!‑1). */
seq_t a(idx_t n)
{
    /* Find the row r containing n.  0! = 1, 1! = 1, 2! = 2, ... */
    idx_t row  = 0;     /* current row (starting at 0) */
    idx_t fact = 1;     /* row!  (fact is 0! = 1 at start) */
    idx_t cum  = 0;     /* cumulative length of rows before current row */

    /* Find the first row where n < cum + fact.  We use n < cum+fact
       as the termination condition to avoid overflow of fact when it
       grows beyond the bounds of idx_t.  For typical test ranges
       (n <= 10000) the loop finishes quickly. */
    while (n >= cum + fact) {
        cum += fact;          /* skip current row */
        row++;                /* move to next row */
        fact *= row;          /* row! = (row-1)! * row */
    }

    /* Position of n within this row (0‑based). */
    idx_t pos = n - cum;
    if (row == 0)
        return (seq_t)0;      /* row 0 has only the element 0 */

    /* Pre‑compute factorials up to (row‑1)! for digit extraction. */
    idx_t fact_arr[21];            /* row will never exceed 20 for n < 2^64 */
    fact_arr[0] = 1;
    for (idx_t i = 1; i < row; ++i)
        fact_arr[i] = fact_arr[i - 1] * i;

    /* Extract factorial digits and accumulate their sum. */
    seq_t sum = 0;
    for (idx_t i = row; i > 0; --i) {
        idx_t div   = fact_arr[i - 1];   /* (i‑1)! */
        idx_t digit = pos / div;
        sum += (seq_t)digit;
        pos  -= digit * div;
    }

    return sum;
}
