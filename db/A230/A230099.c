/*
 * A230099  a(n) = n + (product of digits of n).
 * https://oeis.org/A230099
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, look
 * Formula:   a(n) = n + product of digits of n
 * Range:     u8 n <= 188, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    seq_t prod = digit_product((seq_t)n);
    return (seq_t)n + prod;
}
