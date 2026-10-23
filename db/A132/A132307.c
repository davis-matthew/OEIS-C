/*
 * A132307  2*A007318^(2) - A000012.
 * https://oeis.org/A132307
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = 2^(r-j+1)*C(r,j) - 1, * where r is the largest integer
 *            with r(r+1)/2 ≤ n, * and j = n - r(r+1)/2
 * Range:     u8 n <= 21, u16 n <= 54, u32 n <= 54, u64 n <= 54, u128 n <= 54,
 *            big any n
 * Helpers:   binomial, pow2
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/pow2.h"

seq_t a(idx_t n)
{
    /* Find the row r such that T(r) ≤ n < T(r+1),   T(r)=r(r+1)/2. */
    idx_t low = 0, high = n + 1;  /* high is exclusive */
    while (low < high) {
        idx_t mid = (low + high) >> 1;
        __int128 t = (__int128)mid * (mid + 1) / 2;
        if ((idx_t)t <= n)
            low = mid + 1;
        else
            high = mid;
    }
    idx_t r = low - 1;               /* largest r with T(r) ≤ n   */
    idx_t start = (r * (r + 1)) / 2; /* index where row r starts    */
    idx_t j = n - start;             /* column index in the row    */

    seq_t bin   = binomial(r, j);               /* C(r,j)        */
    idx_t exp   = r - j + 1;                    /* exponent part */
    seq_t powv  = pow2(exp);                    /* 2^exp         */
    seq_t prod  = bin * powv;                   /* C(r,j) * 2^exp*/
    return prod - (seq_t)1;                    /* subtract 1    */
}
