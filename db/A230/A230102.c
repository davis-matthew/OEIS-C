/*
 * A230102  a(0)=1; thereafter a(n+1) = a(n) + (product of digits of a(n)).
 * https://oeis.org/A230102
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(0)=1; a(n+1)=a(n)+digit_product(a(n))
 * Range:     u8 n <= 50, u16 n <= 50, u32 n <= 50, u64 n <= 50, u128 n <= 50,
 *            big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_product.h"

seq_t a(idx_t n) {
    seq_t val = 1;                 /* a(0) */
    for (idx_t i = 0; i < n; ++i) {
        val += digit_product(val);
    }
    return val;
}
