/*
 * A218016  Triangle, read by rows, where T(n,k) = k!*C(n, k)*5^(n-k) for
 *          n>=0, k=0..n.
 * https://oeis.org/A218016
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = k!*C(r,k)*5^(r-k) * where r = floor((sqrt(8*n+1)-1)/2) *
 *            and k = n - r*(r+1)/2 (0 ≤ k ≤ r)
 * Range:     u8 n <= 7, u16 n <= 27, u32 n <= 81, u64 n <= 221, u128 n <= 617,
 *            big any n
 * Helpers:   binomial, factorial, floor_sqrt, power5
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/floor_sqrt.h"
#include "../utils/binomial.h"
#include "../utils/factorial.h"
#include "../utils/power5.h"

seq_t a(idx_t n)
{
    /* 1. Find row r and column k from index n */
    seq_t tmp = (seq_t)8 * (seq_t)n + 1;        /* 8*n + 1 */
    seq_t s   = floor_sqrt(tmp);               /* floor(sqrt(8*n+1)) */
    idx_t r   = ((idx_t)s - 1) / 2;             /* r = floor((sqrt(8*n+1)-1)/2) */
    idx_t start = r * (r + 1) / 2;              /* first index of row r */
    idx_t k   = n - start;                      /* column index */

    /* 2. Compute the triangle entry T(r,k) */
    seq_t res = binomial(r, k);                 /* C(r,k) */
    res *= factorial(k);                       /* * k!   */
    res *= power5(r - k);                       /* * 5^(r-k) */

    return res;
}
