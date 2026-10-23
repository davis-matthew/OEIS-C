/*
 * A031347  Multiplicative digital root of n (keep multiplying digits of n
 *          until reaching a single digit).
 * https://oeis.org/A031347
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base, easy, nice
 * Formula:   not stated
 * Range:     u8 n <= 745, u16 n <= 10000, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digit_product
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computation: multiplicative digital root by iterating digit product until single digit */
#include <stdbool.h>
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    seq_t x = (seq_t)n;
    while (x >= 10) {
        x = digit_product(x);
    }
    return x;
}
