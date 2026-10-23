/*
 * A153764  Triangle T(n,k), 0 <= k <= n, read by rows, given by
 *          [1,0,-1,0,0,0,0,0,0,0,0,...] DELTA [0,1,0,0,0,0,0,0,0,0,...] where
 *          DELTA is the operator defined in A084938.
 * https://oeis.org/A153764
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = T(r,k) with r = max{m | m(m+1)/2 <= n}, k = n - r(r+1)/2,
 *            T(r,k) = binomial( floor((r+k-1)/2), k ) (where binomial(n,k) is
 *            the standard binomial coefficient)
 * Range:     u8 n <= 141, u16 n <= 392, u32 n <= 1080, u64 n <= 1080,
 *            u128 n <= 1080, big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    /* Find r  with  r*(r+1)/2 <= n < (r+1)*(r+2)/2  */
    idx_t low = 0;
    idx_t high = 1;
    /* increase upper bound until it exceeds n */
    while ((unsigned __int128)high * (high + 1) / 2 <= n) {
        high <<= 1;
    }
    /* binary search */
    while (low < high) {
        idx_t mid = (low + high + 1) >> 1;          /* upper mid to avoid infinite loop */
        if ((unsigned __int128)mid * (mid + 1) / 2 <= n)
            low = mid;
        else
            high = mid - 1;
    }
    idx_t r = low;

    /* compute column k */
    idx_t row_start = (unsigned __int128)r * (r + 1) / 2;
    idx_t k = n - (idx_t)row_start;

    /* compute m = floor((r + k - 1)/2) */
    idx_t m;
    if (r == 0 && k == 0)
        m = 0;                       /* special case: T(0,0)=1 */
    else
        m = (r + k - 1) / 2;          /* r+k-1 >= 1 for all other cases */

    return binomial(m, k);
}
