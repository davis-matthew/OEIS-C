/*
 * A124929  Triangle read by rows: T(n,k) = (2^k-1)*binomial(n-1,k-1)
 *          (1<=k<=n).
 * https://oeis.org/A124929
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the n‑th element of the triangle T(r,k) =
 *            (2^k-1)·C(r-1,k-1) read by rows. For a given linear index n
 *            (n≥1) the row r is the unique integer with r(r-1)/2 < n ≤
 *            r(r+1)/2, and k = n – r(r-1)/2. The required term is then a(n) =
 *            (2^k-1) * binomial(r-1,k-1)
 * Range:     u8 n <= 24, u16 n <= 73, u32 n <= 265, u64 n <= 929,
 *            u128 n <= 1275, big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>                         /* idx_t and seq_t types */
#include "../utils/binomial.h"              /* seq_t binomial(idx_t,idx_t) */
#include "../utils/pow2.h"                  /* seq_t pow2(idx_t) */

seq_t a(idx_t n)
{
    /* locate the row r (1‑based) that contains the n‑th element */
    idx_t r = 1;           /* current row */
    idx_t tri = 1;         /* triangular number r(r+1)/2 of the current row */

    while (tri < n) {
        ++r;
        tri += r;          /* tri = r(r+1)/2 after the increment */
    }

    /* k = column in row r   (1‑based) */
    idx_t prev_tri = tri - r;          /* (r-1)r/2 */
    idx_t k = n - prev_tri;            /* 1 ≤ k ≤ r */

    /* compute the term using the formula */
    seq_t bin = binomial(r - 1, k - 1); /* C(r-1, k-1) */
    seq_t p2 = pow2(k);                /* 2^k */

    return (p2 - (seq_t)1) * bin;
}
