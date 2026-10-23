/*
 * A101157  Let j be the smallest integer for which n+(n+1)+...+(n+j) is a
 *          square, say k^2; then a(n)=k.
 * https://oeis.org/A101157
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 9, u16 n <= 163, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   isqrt
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search for the smallest j ≥ 0 such that n + (n+1) + … + (n+j) is a square.
   The function returns the square root of that sum, i.e. a(n). */
#include <stdbool.h>
#include "../utils/isqrt.h"

seq_t a(idx_t n) {
    /* When n is the starting integer, we incrementally add successive
       integers until the cumulative sum becomes a perfect square. */
    seq_t sum = 0;
    for (idx_t j = 0;; ++j) {
        /* sum of n + (n+1) + … + (n+j) */
        sum += (seq_t)(n + j);
        seq_t k = isqrt(sum);
        if (k * k == sum)
            return k;
    }
}
