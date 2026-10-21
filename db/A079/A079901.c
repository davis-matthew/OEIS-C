/*
 * A079901  Triangle of powers, T(n,k) = n^k, 0 <= k <= n, read by rows.
 * https://oeis.org/A079901
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = r^k where r = floor((sqrt(8*n+1)-1)/2) // row index k = n
 *            - r*(r+1)/2 // column index (row r has r+1 terms T(r,k)=r^k for
 *            0≤k≤r)
 * Range:     u8 n <= 13, u16 n <= 33, u32 n <= 64, u64 n <= 151,
 *            u128 n <= 404, big any n
 * Helpers:   integer_sqrt_u128
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/integer_sqrt_u128.h"

seq_t a(idx_t n)
{
    /* --- find the row r --- */
    __int128 v = (__int128)n * 8 + 1;          /* v = 8*n + 1 */
    idx_t sq = integer_sqrt_u128(v);           /* floor(sqrt(v)) */
    idx_t r  = (sq - 1) / 2;                   /* floor((sqrt(8*n+1)-1)/2) */

    /* --- offset within the row --- */
    idx_t start = r * (r + 1) / 2;             /* first index of row r */
    idx_t k      = n - start;                  /* column index in that row */

    /* --- compute r^k --- */
    seq_t result = 1;
    seq_t base   = (seq_t)r;
    for (idx_t i = 0; i < k; ++i) {
        result *= base;
    }

    return result;
}
