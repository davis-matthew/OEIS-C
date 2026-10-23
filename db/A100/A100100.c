/*
 * A100100  Triangle T(n,k) = binomial(2*n-k-1, n-k) read by rows.
 * https://oeis.org/A100100
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nonn, tabl
 * Formula:   a(n) = binomial(2*r - k - 1, r - k) where r = floor(
 *            (sqrt(8*n+1)-1)/2 ), k = n - r*(r+1)/2. Special case: a(0) = 1
 * Range:     u8 n <= 14, u16 n <= 44, u32 n <= 170, u64 n <= 629,
 *            u128 n <= 2277, big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Complexity: O(√n) per call, well below 2 s for n ≤ 10 000. */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n) {
    /* Special case for the first element */
    if (n == 0u) {
        return (seq_t)1;
    }

    /* Find the row r such that r*(r+1)/2 ≤ n < (r+1)*(r+2)/2 */
    idx_t r = 0;
    while (1) {
        unsigned __int128 val = (unsigned __int128)(r + 1) * (r + 2) / 2;
        if (val > n) {
            break;
        }
        r++;
    }

    /* Compute the offset k within that row */
    idx_t start = (idx_t)((unsigned __int128)r * (r + 1) / 2);
    idx_t k = n - start;

    /* Compute the binomial coefficient T(r,k) */
    idx_t N = 2 * r - k - 1;  /* 2*r - k - 1 ≥ 0 for r ≥ 1 */
    idx_t K = r - k;          /* 0 ≤ K ≤ N */

    return binom(N, K);
}
