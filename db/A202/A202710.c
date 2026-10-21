/*
 * A202710  Triangle read by rows. T(n, k) = coefficient of x^n in the Taylor
 *          expansion of [((1 - x - 2*x^2 - sqrt(1 - 2*x -
 *          3*x^2))/(2*x^2))]^k.
 * https://oeis.org/A202710
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 21, u16 n <= 57, u32 n <= 66, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*  Formula used:
    T( r , k ) = coeff of x^r in ( M(x) )^k,    k ≥ 1 , 1 ≤ k ≤ r
    where M(x) is the Motzkin generating function.
    Explicitly:
        T( r , k ) = k * Σ_{i=0}^{r-k}   C(k+i, r-k-i) * C(r, i) / (k+i)
    The sequence a(n) is read by rows:
        a(n) = T( r , k )   where n = r(r-1)/2 + k   (1 ≤ k ≤ r)     */

#include <stdint.h>
#include <stdlib.h>
#include "../utils/binomial.h"    /* provides: seq_t binomial(idx_t n, idx_t k) */

seq_t a(idx_t n)
{
    if (n == 0)
        return 0;                 /* sequence is 1‑indexed */

    /* Find row r such that 1+2+…+r = r(r+1)/2 ≥ n */
    idx_t r = 1;
    while (r * (r + 1) / 2 < n)
        ++r;

    /* Column index k */
    idx_t k = n - r * (r - 1) / 2;   /* 1 ≤ k ≤ r */

    /* Sum the terms of the closed form */
    seq_t sum = 0;
    idx_t limit = r - k;             /* maximum i = r-k */

    for (idx_t i = 0; i <= limit; ++i)
    {
        /* C(k+i, r-k-i) * C(r, i)   then multiply by k, divide by (k+i) */
        seq_t term = binomial(k + i, r - k - i);
        term *= binomial(r, i);
        term *= k;                   /* k * term will be divisible by (k+i) */
        term /= (k + i);
        sum += term;
    }
    return sum;
}
