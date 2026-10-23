/*
 * A177939  Array t(n,m)=(n*m)!/(n + m - 1)! read by antidiagonals.
 * https://oeis.org/A177939
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = ((i*j)!)/(i+j-1)! where (i,j) is the pair on the
 *            antidiagonal traversal of t(n,m) = (nm)!/(n+m-1)!. The index n
 *            starts at 1. This implementation maps n to the appropriate (i,j)
 *            and evaluates the product (i+j) * … * (i*j). It uses only
 *            integer arithmetic and works for any seq_t build (fixed...
 * Range:     u8 n <= 11, u16 n <= 17, u32 n <= 30, u64 n <= 38, u128 n <= 38,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* 1. Find the antidiagonal d (with sum d) that contains the term.
       For d the number of terms up to that diagonal is d(d-1)/2. */
    idx_t d   = 1;          /* current diagonal sum   */
    idx_t terms = 0;        /* total terms seen so far */

    while (terms < n) {
        ++d;                 /* move to next diagonal */
        terms += d - 1;      /* add its length (d-1) */
    }

    /* 2. Determine the offset within this diagonal. */
    idx_t terms_before = terms - (d - 1);      /* terms prior to this diag */
    idx_t offset = n - terms_before - 1;       /* 0‑based offset inside diag */

    /* 3. Convert offset to coordinates (i,j).  Diagonal d has pairs
       (1,d-1), (2,d-2), ..., (d-1,1). */
    idx_t i = offset + 1;
    idx_t j = d - i;

    /* 4. Evaluate the product (i+j)…(i*j).  If i*j <= i+j-1, the ratio
       equals 1. */
    idx_t prod_start = i + j;           /* first factor: s+1, where s=i+j-1 */
    idx_t prod_end   = i * j;           /* last factor: m         */
    if (prod_end <= prod_start - 1) {
        return (seq_t)1;
    }

    seq_t res = (seq_t)1;
    for (idx_t k = prod_start; k <= prod_end; ++k) {
        res *= (seq_t)k;
    }
    return res;
}
