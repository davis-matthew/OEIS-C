/*
 * A118373  Product of decimal digits of two successive natural numbers.
 * https://oeis.org/A118373
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = digit_product(n-1) * digit_product(n)
 * Range:     u8 n <= 28, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_product.h"

seq_t a(idx_t n) {
    /* OEIS offset is 1, thus a(1) corresponds to n=0 */
    if (n == 0) return 0; /* safety for unexpected n==0 */
    seq_t p1 = digit_product((seq_t)(n - 1));
    seq_t p2 = digit_product((seq_t)n);
    return p1 * p2;
}
