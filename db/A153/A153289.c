/*
 * A153289  A triangular sequence designed with row sums near 3^n: t(n,m)=If[m
 *          == 0 || m == n, Floor[3^n/2^n], Floor[(3^n/2^n)*Binomial[n, m]] +
 *          1].
 * https://oeis.org/A153289
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = t(r,c) where r = floor((sqrt(8n+1)-1)/2) and c = n -
 *            r*(r+1)/2; t(r,0)=t(r,r)=floor((3^r)/(2^r)), otherwise
 *            t(r,c)=floor(((3^r)/(2^r))*binom(r,c))+1. (Standard integer
 *            arithmetic. The triangular table is traversed row‑wise.)
 * Range:     u8 n <= 10, u16 n <= 30, u32 n <= 59, u64 n <= 59, u128 n <= 59,
 *            big any n
 * Helpers:   binom, pow2, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow3.h"
#include "../utils/pow2.h"
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row r such that n lies in the r‑th row of the triangle
       (0‑based indices).  The number of elements up to row r-1 is
       r*(r+1)/2, so we increase r while that value is <= n. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Index of the first element of row r. */
    idx_t prefix = r * (r + 1) / 2;
    /* Column index within the row. */
    idx_t c = n - prefix;

    /* Compute 3^r and 2^r.  The seq_t type supports usual operators. */
    seq_t pow3_val = pow3(r);
    seq_t pow2_val = pow2(r);

    /* Value at the ends of the row. */
    seq_t edge_val = pow3_val / pow2_val;   /* floor((3^r)/(2^r)) */

    if (c == 0 || c == r)          /* first or last element of the row */
        return edge_val;

    /* Interior elements.  Compute floor(((3^r)/(2^r))*binom(r,c))+1. */
    seq_t bin = binom(r, c);
    seq_t prod = pow3_val * bin;              /* (3^r) * binom(r,c) */
    seq_t val  = (prod / pow2_val) + 1;        /* floor division + 1 */

    return val;
}
