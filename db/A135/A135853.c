/*
 * A135853  A103516 * A007318 as an infinite lower triangular matrix.
 * https://oeis.org/A135853
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 30, u16 n <= 125, u32 n <= 478, u64 n <= 1274,
 *            u128 n <= 1274, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  A135853 : Flattened triangle T(n,k) where
    T(n,0) = 2·(n+1) for n>0 ,  T(0,0)=1
    T(n,k) = (k+1)·C(n+1,k+1)   for 0<k<n
    T(n,n) = n+1
    The index n is interpreted as the flattened position of the lower
    triangular matrix (row‑major order).  The routine computes the
    value at that position. */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Handle the first entry separately – the general formula gives 2,
       but the OEIS entry defines T(0,0)=1. */
    if (n == 0)
        return (seq_t)1;

    /* Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2.
       For n up to a few 10 000 this simple loop is far below the
       time limit. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row. */
    idx_t prev = r * (r + 1) / 2;
    idx_t c = n - prev;

    /* Compute the matrix entry.  All arithmetic is performed in
       seq_t to avoid intermediate overflow. */
    if (c == 0) {
        /* T(r,0) = 2·(r+1)  for r>0 */
        return (seq_t)(2 * (seq_t)(r + 1));
    } else {
        /* General entry: (c+1)·C(r+1,c+1).  The binomial
           function returns a seq_t. */
        return (seq_t)(c + 1) * binomial((idx_t)(r + 1), (idx_t)(c + 1));
    }
}
