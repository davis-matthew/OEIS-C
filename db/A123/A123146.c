/*
 * A123146  Triangle T(n, k) = (binomial(n,2))! / (k! * abs(k+1 -
 *          binomial(n,2))!), read by rows.
 * https://oeis.org/A123146
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, easy
 * Formula:   a(n) = T(r,k) where r = floor((sqrt(8*n+1)-1)/2), k = n -
 *            r*(r+1)/2, M = r*(r+1)/2, T(r,k) = binomial(M,k)*(M-k) for k !=
 *            M, otherwise T(r,M) = 1
 * Range:     u8 n <= 11, u16 n <= 24, u32 n <= 52, u64 n <= 133,
 *            u128 n <= 348, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2.
       n is up to about 10000, a linear search is cheap. */
    idx_t r = 0;
    while ((r + 1) * (r + 2) / 2 <= n)
        ++r;

    /* Column index within the row */
    idx_t k = n - r * (r + 1) / 2;

    /* M = binomial(r+1,2) = r*(r+1)/2 */
    idx_t M = r * (r + 1) / 2;

    /* For the very small rows (r=0,1) the last column gives 1.
       This is handled explicitly. */
    if (k == M)
        return (seq_t)1;

    /* Compute T(r,k) = binomial(M, k) * (M - k) */
    seq_t res = binomial((idx_t)M, (idx_t)k);
    res *= (M - k);
    return res;
}
