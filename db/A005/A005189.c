/*
 * A005189  Number of n-term 2-sided generalized Fibonacci sequences.
 * https://oeis.org/A005189
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 2*(n-1)*a(n-1) - (n-2)^2*a(n-2), a(0) = a(1) = a(2) = 1
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 19, u128 n <= 32,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    seq_t prev2, prev1, cur;
    idx_t i;
    if (n <= 2) return (seq_t)1;
    prev2 = (seq_t)1; /* a(1) */
    prev1 = (seq_t)1; /* a(2) */
    for (i = 3; i <= n; i++) {
        seq_t c1 = (seq_t)(i - 1);
        seq_t c2 = (seq_t)(i - 2);
        cur = (seq_t)2 * c1 * prev1 - c2 * c2 * prev2;
        prev2 = prev1;
        prev1 = cur;
    }
    return prev1;
}
