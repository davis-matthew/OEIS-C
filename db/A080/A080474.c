/*
 * A080474  a(n) = product of sum of taking n-1 numbers from the next n
 *          numbers. The next n numbers can be grouped like this (1), (2,3),
 *          (4,5,6), (7,8,9,10),... . Let N be the sum of all the members of
 *          the n-th group. Let k be a member and f(k) = N - k. Then a(n) =
 *          the product of all f(k) for k taking all me...
 * https://oeis.org/A080474
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = product_{k=start}^{start+n-1} (N - k) where * N =
 *            n*(n^2+1)/2, start = n*(n-1)/2 + 1. * Equivalently for n ≥ 2: *
 *            m = n*(n^2 - n + 2)/2 - 1 * a(n) = falling factorial (m)_n =
 *            ∏_{i=0}^{n-1} (m - i) * For n = 1, a(1) = 1 by definition
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 5, u64 n <= 8, u128 n <= 12,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Special case n = 1 */
    if (n == 1)
        return (seq_t)1;

    /* Compute m = n*(n^2 - n + 2)/2 - 1 */
    seq_t nn = (seq_t)n;
    seq_t part = nn * nn - nn + 2;          /* part = n^2 - n + 2, always even */
    seq_t m = (nn * part) / 2 - 1;          /* m fits in seq_t for n up to ~10^4 */

    /* Compute the falling factorial product */
    seq_t prod = (seq_t)1;
    for (idx_t i = 0; i < n; ++i) {
        prod *= (m - i);
    }
    return prod;
}
