/*
 * A124847  Triangle read by rows: T(n,k) = k(k+1)*binomial(n-1, k-1)/2 (1 <=
 *          k <= n).
 * https://oeis.org/A124847
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, tabl
 * Formula:   a(n) = k(k+1)/2 * binomial(r-1, k-1), where r is the minimum
 *            integer with r(r+1)/2 >= n, and k = n – r(r-1)/2 (1‑based column
 *            in row r)
 * Range:     u8 n <= 31, u16 n <= 62, u32 n <= 62, u64 n <= 62, u128 n <= 62,
 *            big any n
 * Helpers:   binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;   /* out of range, safe guard */

    /* Find the row r by accumulating triangular numbers.
       Use unsigned __int128 to avoid overflow on the sum. */
    idx_t r = 1;
    unsigned __int128 sum = 1;          /* triangular of r */
    while (sum < n) {
        ++r;
        sum += r;
    }

    /* Previous sum (triangular of r-1). */
    unsigned __int128 prev_sum = sum - r;

    /* Column index k (1‑based). */
    idx_t k = (idx_t)(n - prev_sum);

    /* Compute the term. */
    seq_t bin = binomial(r - 1, k - 1);
    seq_t t   = (seq_t)k * (k + 1) / 2;
    return t * bin;
}
