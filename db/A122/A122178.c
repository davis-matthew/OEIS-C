/*
 * A122178  Triangle, read by rows, where T(n,k) = C( n*(n+1)/2 + n-k - 1,
 *          n-k), for n>=k>=0.
 * https://oeis.org/A122178
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = C( n*(n+1)/2 + n - k - 1 , n - k ), where n is the
 *            flattened index, and k = n - r*(r+1)/2 with r(n) = floor(
 *            (sqrt(8*n+1)-1)/2 )
 * Range:     u8 n <= 9, u16 n <= 20, u32 n <= 44, u64 n <= 45, u128 n <= 45,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* helper for binomial coefficient */
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    /* a(0) = 1 (the triangle starts with a single 1) */
    if (n == 0)
        return (seq_t)1;

    /* Find the row r such that r*(r+1)/2 <= n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    /* linear search is fine for n <= 10 000 (≈ 140 iterations max) */
    while ((__uint128_t)r * (r + 1) / 2 <= n)
        r++;
    r--;                    /* last r satisfied the inequality */

    /* offset = total number of entries before row r */
    idx_t offset = (idx_t)((__uint128_t)r * (r + 1) / 2);

    /* column index k in the current row */
    idx_t k = n - offset;

    /* Binomial parameters */
    idx_t K = r - k;                         /* n - k in the OEIS formula */
    idx_t N = offset + K - 1;                /*   n*(n+1)/2 + n - k - 1 */

    return binom(N, K);                      /* compute C(N,K) */
}
