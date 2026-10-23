/*
 * A193871  Square array T(n,k) = k^n - k + 1 read by antidiagonals.
 * https://oeis.org/A193871
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = k^n - k + 1 where (n,k) are obtained by reading the
 *            infinite array T(n,k) = k^n - k + 1 along its antidiagonals in
 *            decreasing row order (bottom-to-top)
 * Range:     u8 n <= 30, u16 n <= 59, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   big_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* the sequence is defined for n >= 1 (offset 1). */
    if (n == 0) return 0;          /* guard against out‑of‑range calls */

    /* Find the diagonal d such that the n-th term lies on it.
       The number of elements up to diagonal d (inclusive) is
       t(d) = (d+1)(d+2)/2.  We iterate until this count reaches or
       exceeds n. */
    idx_t d   = 0;          /* diagonal index, zero‑based */
    idx_t cum = 1;          /* cumulative count t(d) for current d */
    while (cum < n) {
        ++d;
        cum += d + 1;       /* t(d) = t(d-1) + (d+1)         */
    }

    /* start = first term index on this diagonal:
          start = t(d-1) + 1 = cum - d.        */
    idx_t start = cum - d;
    idx_t offset = n - start;       /* 0‑based offset within the diagonal   */

    /* For decreasing row order within the diagonal:
           row0 = d - offset   (zero‑based row index)
           col0 = offset       (zero‑based column index)   */
    idx_t row0 = d - offset;
    idx_t col0 = offset;

    /* Convert to 1‑based indices for the formula.               */
    seq_t row = (seq_t)(row0 + 1);   /* n in the definition */
    seq_t col = (seq_t)(col0 + 1);   /* k in the definition */

    /* Compute k^n - k + 1.  big_pow works with seq_t arguments. */
    seq_t val = big_pow(col, row);
    val -= col;
    val += 1;
    return val;
}
