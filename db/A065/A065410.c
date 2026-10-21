/*
 * A065410  Number of labeled, rooted, binary trees with internal nodes
 *          labeled from {1, ...,n} and leaves labeled from {0,1} such that
 *          for any path from the root to a leaf, the internal nodes receive
 *          distinct labels. In other words, the number of decision trees on n
 *          Boolean variables.
 * https://oeis.org/A065410
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  easy, nice, nonn
 * Formula:   a(n) = n*(a(n-1))^2 + 2, with a(0) = 2
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdlib.h>

seq_t a(idx_t n) {
    seq_t curr = 2;                     /* a(0) */
    if (n == 0) return curr;

    for (idx_t i = 1; i <= n; ++i) {
        seq_t sq = curr * curr;          /* (a(n-1))^2 */
        curr = (seq_t)i * sq + 2;        /* a(n) = i * (a(n-1))^2 + 2 */
    }
    return curr;
}
