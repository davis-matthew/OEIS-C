/*
 * A121106  Trajectory of 6788 under "x -> product of digits of x" map.
 * https://oeis.org/A121106
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(1) = 6788, a(n) = digit_product(a(n-1)) for n > 1
 * Range:     u8 none, u16 n <= 7, u32 n <= 7, u64 n <= 7, u128 n <= 7,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_product.h"

seq_t a(idx_t n) {
    seq_t cur = (seq_t)6788;
    if (n <= 1) return cur;
    idx_t i;
    for (i = 1; i < n && cur != (seq_t)0; ++i) {
        cur = digit_product(cur);
    }
    return cur;
}
