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
 * Range:     i8 n <= 287, i16 n <= 9917, i32 n <= 9917, i64 n <= 9917,
 *            i128 n <= 9917, big any n
 * Helpers:   digit_product, digits
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/digit_product.h"
#include "../utils/digits.h"

seq_t a(idx_t n)
{
    seq_t prod = digit_product((seq_t)n);
    if (prod == 0) return 0;

    int dcnt = digits(n);
    if (dcnt & 1) prod = -prod;
    return prod;
}
