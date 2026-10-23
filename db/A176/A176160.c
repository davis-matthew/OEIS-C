/*
 * A176160  Triangle read by rows: T(n,k) = (1 + 4*binomial(n, k))^n.
 * https://oeis.org/A176160
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, tabl, less
 * Formula:   a(n) is the n-th entry of the triangle read by rows: T(n,k) = (1
 *            + 4*binomial(n,k))^n
 * Range:     u8 n <= 6, u16 n <= 10, u32 n <= 22, u64 n <= 35, u128 n <= 35,
 *            big any n
 * Helpers:   big_pow, binomial
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binomial.h"
#include "../utils/big_pow.h"

seq_t a(idx_t n)
{
    /* Find the row r such that cumulative number of terms up to r-1
       is <= n < cumulative number of terms up to r. */
    idx_t low  = 0;
    idx_t high = 1;
    /* Increase high until the cumulative count exceeds n */
    while (((high + 1) * (high + 2)) / 2 <= n)
        high <<= 1;

    /* Binary search for minimal r with cumulative > n */
    while (low + 1 < high) {
        idx_t mid = low + (high - low) / 2;
        if (((mid + 1) * (mid + 2)) / 2 <= n)
            low = mid;
        else
            high = mid;
    }
    idx_t r = high;                     /* row index */
    idx_t k = n - (r * (r + 1)) / 2;    /* column index within the row */

    /* Compute T(r,k) = (1 + 4*binomial(r,k))^r */
    seq_t binom = binomial(r, k);
    seq_t base  = (seq_t)1 + (seq_t)4 * binom;
    return big_pow(base, (seq_t)r);
}
