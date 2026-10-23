/*
 * A016035  a(n) = Sum_{j|n, 1 < j < n} phi(j). Also a(n) = n - phi(n) - 1 for
 *          n > 1.
 * https://oeis.org/A016035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = 0 if n == 1, else n - phi(n) - 1
 * Range:     u8 n <= 359, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   phi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/phi.h"

seq_t a(idx_t n) {
    if (n == 1) {
        return 0;
    }
    return (seq_t)n - phi(n) - 1;
}
