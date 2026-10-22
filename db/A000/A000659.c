/*
 * A000659  a(n) = 2^a(n-1) + a(n-2).
 * https://oeis.org/A000659
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2^a(n-1) + a(n-2), a(0) = 0, a(1) = 0
 * Range:     u8 n <= 5, u16 n <= 5, u32 n <= 5, u64 n <= 6, u128 n <= 6,
 *            big any n
 * Helpers:   pow2_by_doubling
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/pow2_by_doubling.h"

seq_t a(idx_t n) {
    seq_t prev2 = (seq_t)0; /* a(0) */
    seq_t prev1 = (seq_t)0; /* a(1) */
    idx_t k;
    if (n == 0) return prev2;
    for (k = 2; k <= n; k++) {
        seq_t cur = pow2_by_doubling(prev1) + prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
