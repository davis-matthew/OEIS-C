/*
 * A197208  Triangular array: T(n,k) = sqrt(C(n-1,k-1)*C(n-1,k)*C(n,k+1)*
 *          C(n+1,k+1)*C(n+1,k)*C(n,k-1)), where C(n,k) = binomial(n,k).
 * https://oeis.org/A197208
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   not stated
 * Range:     u8 n <= 8, u16 n <= 24, u32 n <= 83, u64 n <= 286,
 *            u128 n <= 1012, big any n
 * Helpers:   binomial, floor_sqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdbool.h>
#include "../utils/binomial.h"
#include "../utils/floor_sqrt.h"

/* a(n) = T(r,k) where (r,k) is the r‑th row (r ≥ 2) and the kth entry
 * (1 ≤ k ≤ r‑1) of the triangular array T(r,k) defined by
 *
 *   T(r,k) = C(r-1,k-1) * C(r,k+1) * C(r+1,k)
 *
 * (the array is listed row by row).  For n ≥ 2,
 * let t = n‑1 be the 1‑based linear index within the row‑major order.
 * The smallest r with r·(r‑1)/2 ≥ t is the row, and
 * k = t – (r‑2)(r‑1)/2 is the column.
 *
 * This algorithm runs in O(√n) per call and uses only integer arithmetic. */

seq_t a(idx_t n)
{
    if (n < 2)
        return 0;          /* behaviour for n < 2 is undefined */

    /* The linear index t (1‑based) in the row‑major enumeration */
    idx_t t = n - 1;

    /* Find the row r: r*(r-1)/2 ≥ t. */
    /* Solve r² – r – 2t ≥ 0. */
    seq_t val = (seq_t)1 + (seq_t)8 * (seq_t)t;          /* 8t+1 fits in seq_t */
    seq_t s   = floor_sqrt(val);                       /* ⌊√(8t+1)⌋          */
    idx_t r   = ((idx_t)1 + (idx_t)s) / 2;             /* initial estimate   */
    while (r * (r - 1) / 2 < t)
        r++;                                         /* adjust if needed */

    /* Column index k = t – previous cumulative count. */
    idx_t prev_cum = (r - 2) * (r - 1) / 2;           /* cumulative up to row r‑1 */
    idx_t k = t - prev_cum;                           /* 1 ≤ k ≤ r‑1 */

    /* Compute the three binomial coefficients for T(r,k). */
    seq_t c1 = binomial(r - 1, k - 1);                 /* C(r-1,k-1)          */
    seq_t c2 = binomial(r,     k + 1);                 /* C(r,k+1)            */
    seq_t c3 = binomial(r + 1, k);                     /* C(r+1,k)            */

    return c1 * c2 * c3;
}
