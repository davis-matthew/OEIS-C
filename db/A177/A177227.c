/*
 * A177227  Triangle, read by rows, T(n, k) = -binomial(n,k) for 0 < k < n,
 *          otherwise T(n, k) = 2.
 * https://oeis.org/A177227
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, tabl, less, easy
 * Formula:   not stated
 * Range:     i8 n <= 38, i16 n <= 177, i32 n <= 609, i64 n <= 1325,
 *            i128 n <= 1325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 *  Triangle read by rows:   T(n,k) = 2                     if k=0 or k=n
 *                           T(n,k) = -binomial(n,k)       otherwise
 *  a(n) is the n‑th element of that triangle.
 */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r that contains index n in the flattened triangle. */
    idx_t r = 0;          /* current row number   */
    idx_t start = 0;      /* starting linear index of current row */

    /* Each row r has length r+1.  We add rows until n lies within the
     * current row.  For n up to 10 000 this loop completes in far less
     * than 100 iterations, so it is comfortably fast. */
    while (n >= start + (r + 1)) {
        start += r + 1;   /* move to the next row */
        ++r;
    }

    /* Column (0‑based) within row r. */
    idx_t k = n - start;

    /* Boundary elements are 2. */
    if (k == 0 || k == r)
        return (seq_t)2;

    /* Inner elements are –C(r,k).  binom(r,k) is non‑negative, so the
     * negation yields the required value. */
    seq_t val = binom(r, k);
    return (seq_t)(-val);
}
