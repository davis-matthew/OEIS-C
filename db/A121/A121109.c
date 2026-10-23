/*
 * A121109  Trajectory of 26888999 under "x -> product of digits of x" map.
 * https://oeis.org/A121109
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, fini, full, base
 * Formula:   a(n) = product_of_digits applied repeatedly to the starting
 *            value 26888999, a(1) = 26888999, a(n) = digit_product(a(n-1))
 *            for n ≥ 2 until the value becomes 0
 * Range:     u8 none, u16 none, u32 n <= 10, u64 n <= 10, u128 n <= 10,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_product.h"

seq_t a(idx_t n) {
    /* Sequence is defined for n ≥ 1. Behaviour for n = 0 is irrelevant. */
    if (n == 0) return 0;   /* safety, not used by OEIS specification */

    /* The trajectory reaches 0 after the 10th term (n = 10).  All later terms are 0. */
    if (n > 10) return 0;

    seq_t cur = 26888999;   /* a(1) */
    if (n == 1) return cur;

    for (idx_t i = 2; i <= n; ++i) {
        cur = digit_product(cur);
        if (cur == 0) break;
    }
    return cur;
}
