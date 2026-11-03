/*
 * A187844  Product of negated digits of n.
 * https://oeis.org/A187844
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  sign, base, look
 * Formula:   a(n) = product of negated digits of n Implementation: compute
 *            the product of the decimal digits of n; if any digit is zero the
 *            result is zero; otherwise the result equals
 *            (-1)^{number_of_digits(n)} times the product of the digits
 * Range:     i8 n <= 287, i16 n <= 25000, i32 n <= 25000, i64 n <= 25000,
 *            i128 n <= 25000, big any n
 * Validated: standard, pass, 105 terms through n = 9917 (2026-11-01)
 * Helpers:   digit_product, num_digits_idx
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/digit_product.h"
#include "../utils/num_digits_idx.h"

seq_t a(idx_t n)
{
    seq_t prod = digit_product((seq_t)n);
    if (prod == 0) return 0;

    int dcnt = num_digits_idx(n);
    if (dcnt & 1) prod = -prod;
    return prod;
}
