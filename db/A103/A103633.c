/*
 * A103633  Triangle read by rows: triangle of repeated stepped binomial
 *          coefficients.
 * https://oeis.org/A103633
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial( floor( row(n)/2 ), row(n)-k ) where row(n) is
 *            the unique r with r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 and k = n –
 *            r*(r+1)/2. The triangle T(r,k)=binomial( floor(r/2), r-k )
 * Range:     u8 n <= 100, u16 n <= 100, u32 n <= 100, u64 n <= 100,
 *            u128 n <= 100, big any n
 * Helpers:   binomial, row_of_n
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/row_of_n.h"

/* OEIS A103633: flattened triangle T(r,k) = binomial(floor(r/2), r-k). */
seq_t a(idx_t n)
{
    /* Find the row and column in the triangle that correspond to the
       given flat index n. */
    idx_t r = row_of_n(n);
    idx_t offset = (r * (r + 1)) / 2;
    idx_t k = n - offset;

    /* Compute T(r,k) = C(floor(r/2), r-k). */
    idx_t N = r >> 1;          /* floor(r/2) */
    idx_t K = r - k;           /* choose r-k from floor(r/2) */
    return binomial(N, K);
}
