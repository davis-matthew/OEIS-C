/*
 * A005154  a(0) = 1, a(1) = 2; thereafter a(n) = 3*a(n-1)^2 - 2*a(n-2)^4.
 * https://oeis.org/A005154
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 3*a(n-1)^2 - 2*a(n-2)^4, a(0) = 1, a(1) = 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prev2 = (seq_t)1;
    seq_t prev1 = (seq_t)2;
    idx_t i;

    if (n == 0) return prev2;
    if (n == 1) return prev1;

    for (i = 2; i <= n; i++) {
        seq_t sq = prev1 * prev1;
        seq_t fourth = prev2 * prev2;
        seq_t cur;
        fourth = fourth * fourth;
        cur = (seq_t)3 * sq - (seq_t)2 * fourth;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
