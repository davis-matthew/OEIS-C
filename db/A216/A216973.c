/*
 * A216973  Exponential Riordan array [x*exp(x),x].
 * https://oeis.org/A216973
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, tabl
 * Formula:   a(n) = (r - c) * binomial(r, c), where r = floor((sqrt(8*n + 1)
 *            - 1) / 2) and c = n - r*(r + 1)/2 (triangular numbers). This
 *            implements the flattened triangle T(n,k) = (n-k) * binomial(n,k)
 *            from OEIS A216973
 * Range:     u8 n <= 38, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/binomial.h"          /* provides static seq_t binomial(idx_t, idx_t) */

seq_t a(idx_t n)
{
    /* Find the unique r with r*(r+1)/2 <= n < (r+1)*(r+2)/2 using binary search. */
    idx_t low = 0, high = n, r = 0;
    while (low <= high) {
        idx_t mid = low + (high - low) / 2;
        unsigned __int128 tri = (unsigned __int128)mid * (mid + 1) / 2;
        if (tri <= n) {
            r = mid;
            low = mid + 1;
        } else {
            if (mid == 0) break;          /* avoid underflow of mid-1 */
            high = mid - 1;
        }
    }

    /* Compute c = n - r*(r+1)/2 */
    idx_t row_start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t c = n - row_start;

    /* a(n) = (r - c) * binomial(r, c) */
    seq_t result = binomial(r, c);
    result *= (seq_t)(r - c);
    return result;
}
