/*
 * A002489  a(n) = n^(n^2), or (n^n)^n.
 * https://oeis.org/A002489
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, easy, nice
 * Formula:   a(n) = n^(n^2), with a(0) = 1
 * Range:     u8 n <= 2, u16 n <= 3, u32 n <= 3, u64 n <= 5, u128 n <= 6,
 *            big any n
 * Helpers:   a002489_powi
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/a002489_powi.h"

seq_t a(idx_t n) {
    if (n == 0) {
        return 1;
    }
    return a002489_powi((seq_t)n, n * n);
}
