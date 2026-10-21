/*
 * A062145  Triangle read by rows: T(n, k) = [z^k] P(n, z) where P(n, z) =
 *          Sum_{k=0..n} binomial(n, k) * Pochhammer(n - k + c, k) * z^k / k!
 *          and c = 4.
 * https://oeis.org/A062145
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   not stated
 * Range:     u8 n <= 16, u16 n <= 49, u32 n <= 178, u64 n <= 610,
 *            u128 n <= 1325, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* 
 * Sequence A062145:  Triangle read by rows
 * T(n,k) = binom(n,k) * binom(n+3,k)
 * a(n) enumerates T(r,k) over rows r=0,1,2,... and columns k=0..r
 * where r is the unique integer for which r*(r+1)/2 <= n < (r+1)*(r+2)/2
 */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* Find the row number r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r   = 0;
    idx_t tri = 0;               /* accumulated terms up to row r-1 */

    while (tri + r + 1 <= n)     /* while adding next row stays <= n */
    {
        tri += r + 1;            /* length of row r is r+1 */
        r++;
    }

    /* Position within the row */
    idx_t k  = n - tri;          /* 0 <= k <= r */

    /* Compute the binomial products in arbitrary precision */
    seq_t b1 = binom(r,     k);  /* binom(r,k)   */
    seq_t b2 = binom(r + 3, k);  /* binom(r+3,k) */

    return b1 * b2;              /* T(r,k) */
}
