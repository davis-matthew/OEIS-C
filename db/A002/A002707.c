/*
 * A002707  Number of ternary trees with n nodes.
 * https://oeis.org/A002707
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, nice, easy
 * Formula:   a(n) = T(n) - T(n-1), where T(n) = 1 + binomial(T(n-1)+2, 3),
 *            T(0) = 1, a(0) = 1 (T(n) counts ternary commutative trees of
 *            height <= n)
 * Range:     u8 n <= 3, u16 n <= 4, u32 n <= 4, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   mc3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/mc3.h"

seq_t a(idx_t n) {
    seq_t prev = 1; /* T(0) */
    if (n == 0) return 1;
    seq_t cur = prev;
    for (idx_t i = 1; i <= n; i++) {
        prev = cur;
        cur = 1 + mc3(prev);
    }
    return cur - prev;
}
