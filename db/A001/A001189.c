/*
 * A001189  Number of degree-n permutations of order exactly 2.
 * https://oeis.org/A001189
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = A000085(n) - 1, where t(n) = t(n-1) + (n-1)*t(n-2), t(0)
 *            = 1, t(1) = 1
 * Range:     u8 n <= 7, u16 n <= 11, u32 n <= 18, u64 n <= 31, u128 n <= 54,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>
#include <stddef.h>
#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prev2 = 1; /* t(i-2) */
    seq_t prev1 = 1; /* t(i-1) */
    idx_t i;
    if (n == 0) return 0;
    for (i = 2; i <= n; i++) {
        seq_t cur = prev1 + (seq_t)(i - 1) * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1 - 1;
}
