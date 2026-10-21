/*
 * A051195  T(2n+2,n), array T as in A050186; a count of aperiodic binary
 *          words.
 * https://oeis.org/A051195
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = C(2n+2, n) - (n even ? C(n+1, n/2) : 0)
 * Range:     u8 n <= 3, u16 n <= 6, u32 n <= 14, u64 n <= 23, u128 n <= 23,
 *            big any n
 * Helpers:   binom
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/binom.h"

seq_t a(idx_t n)
{
    // Compute the central binomial coefficient C(2*n+2, n)
    seq_t total = binom(2*n + 2, n);

    // If n is even, subtract the middle binomial coefficient C(n+1, n/2)
    if ((n & 1) == 0) {
        idx_t k = n / 2;
        seq_t sub = binom(n + 1, k);
        return total - sub;
    }

    // For odd n, no subtraction
    return total;
}
