/*
 * A208510  Triangle of coefficients of polynomials u(n,x) jointly generated
 *          with A029653; see the Formula section.
 * https://oeis.org/A208510
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) is the (offset)-th coefficient in the triangle T, where
 *            T(r,0)=T(r,r-1)=1 and for 1 ≤ k ≤ r-2: T(r,k) =
 *            C(r-1,k)*(2*(r-1)-k)/(r-1). Here r is the smallest integer with
 *            r(r+1)/2 ≥ n, and offset = n - r(r-1)/2 - 1
 * Range:     u8 n <= 31, u16 n <= 78, u32 n <= 78, u64 n <= 78, u128 n <= 78,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <math.h>                       /* for sqrt() */
#include "../utils/binomial.h"           /* for binomial() */

seq_t a(idx_t n)
{
    if (n <= 0)
        return 0;                       /* undefined for n ≤ 0 */

    /* Find r such that r(r+1)/2 >= n.  r ≈ (sqrt(8n+1)-1)/2. */
    idx_t r = (idx_t)((sqrt((double)(8.0 * (long double)n + 1.0)) - 1.0) / 2.0);
    if ( (r * (r + 1)) / 2 < n )
        ++r;                             /* adjust if the estimate is too low */

    /* Previous triangular number t_{r-1} = (r-1)*r/2. */
    idx_t prev = (r - 1) * r / 2;
    idx_t offset = n - prev - 1;        /* column index, 0‑based */

    if (offset == 0 || offset == r - 1)
        return (seq_t)1;                /* outer coefficients are 1 */

    /* Compute T(r, offset) = C(r-1, offset) * (2*(r-1)-offset) / (r-1). */
    seq_t binom = binomial(r - 1, offset);
    seq_t numerator = binom * (seq_t)(2 * (r - 1) - offset);
    seq_t denom = (seq_t)(r - 1);
    return numerator / denom;
}
