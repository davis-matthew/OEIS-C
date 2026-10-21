/*
 * A097610  Triangle read by rows: T(n,k) is number of Motzkin paths of length
 *          n and having k horizontal steps.
 * https://oeis.org/A097610
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 35, u16 n <= 124, u32 n <= 358, u64 n <= 1080,
 *            u128 n <= 1080, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Triangle read by rows: T(n,k) = binom(n,k) * Catalan_{(n-k)/2}
 * where Catalan_m = binom(2m,m)/(m+1).  All other values are 0.
 * a(n) returns the n‑th element of the triangle in row‑major order.
 * Find the row r such that r(r+1)/2 ≤ n < (r+1)(r+2)/2,
 * then let c = n - r(r+1)/2.  Finally compute T(r,c).
 */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Determine row r where n lies.  It satisfies
       r*(r+1)/2 ≤ n < (r+1)*(r+2)/2. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    idx_t start = r * (r + 1) / 2;
    idx_t c = n - start;

    /* For a valid entry we need n-k even.  Here delta = r-c. */
    idx_t delta = r - c;
    if ((delta & 1) != 0)        /* odd → 0 */
        return 0;

    idx_t m = delta >> 1;        /* m = (r-c)/2 */

    /* Compute binomial(r,c).  The helper returns 0 if c>r. */
    seq_t binom_rc = binom(r, c);
    if (binom_rc == 0)
        return 0;

    /* Catalan_{m} = binom(2m,m)/(m+1) */
    seq_t catalan = binom(2 * m, m);
    catalan /= (seq_t)(m + 1);

    /* Result: binom(r,c) * Catalan_m */
    return binom_rc * catalan;
}
