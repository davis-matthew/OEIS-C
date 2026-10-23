/*
 * A047178  Number of nonempty subsets of {1,2,...,n} in which exactly 3/5 of
 *          the elements are <= (n-1)/2.
 * https://oeis.org/A047178
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   * a(n) = sum_{k=1}^{min(floor(L/3), floor(R/2))} binom(L,3k) *
 *            binom(R,2k) * where L = floor((n-1)/2) are the elements ≤
 *            (n-1)/2, * R = n - L are the remaining elements. * The sum
 *            counts all nonempty subsets of {1,…,n} whose size is a multiple
 *            * of 5 and in which exactly 3/5 of the elements belon...
 * Range:     u8 n <= 12, u16 n <= 20, u32 n <= 34, u64 n <= 34, u128 n <= 34,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/binom.h"

seq_t a(idx_t n)
{
    if (n == 0) return 0;
    idx_t L = (n - 1) / 2;          /* lower half count */
    idx_t R = n - L;                /* upper half count */
    idx_t kmax = L / 3;
    if (R / 2 < kmax) kmax = R / 2;
    seq_t total = 0;
    for (idx_t k = 1; k <= kmax; ++k) {
        seq_t left  = binom(L, 3 * k);
        seq_t right = binom(R, 2 * k);
        total += left * right;
    }
    return total;
}
