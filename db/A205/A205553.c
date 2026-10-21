/*
 * A205553  Square array by antidiagonals: C(max(i-1,2j-2),min(i-1,2j-2)),
 *          i>=1, j>=1.
 * https://oeis.org/A205553
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 39, u16 n <= 74, u32 n <= 74, u64 n <= 74, u128 n <= 74,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Enumeration of the square array by antidiagonals (j descending) and
   evaluation of a(i,j) = binomial(max(i-1,2j-2),min(i-1,2j-2)).
   a(n) is defined with offset 1. */
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the diagonal d = i + j (d ≥ 2) that contains the n‑th
       entry, counting d in increasing order and the positions inside
       a diagonal in decreasing j. */
    idx_t d = 2;          /* current diagonal (minimum is 2) */
    idx_t acc = 0;        /* cumulative count of entries in all previous diagonals */

    for (;; ++d) {
        idx_t len = d - 1;              /* number of entries on this diagonal */
        if (n <= acc + len)
            break;
        acc += len;                     /* move to the next diagonal */
    }

    /* Position of n within the found diagonal (1 … len). */
    idx_t pos = n - acc;                /* 1‑based position */

    /* With j descending: j = d - pos,    i = d - j */
    idx_t j = d - pos;
    idx_t i = d - j;                    /* i = d - j, hence i + j = d */

    /* Compute the binomial coefficient: C(max(i-1,2j-2),min(i-1,2j-2)) */
    idx_t a1 = i - 1;                   /* i-1  */
    idx_t a2 = (j << 1) - 2;            /* 2j-2 */
    idx_t maxv = a1 > a2 ? a1 : a2;
    idx_t minv = a1 < a2 ? a1 : a2;

    return binomial(maxv, minv);
}
