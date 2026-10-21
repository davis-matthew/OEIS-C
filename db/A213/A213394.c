/*
 * A213394  The difference between n and the product of the digits of the n-th
 *          prime.
 * https://oeis.org/A213394
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, sign
 * Formula:   a(n) = n - digit_product(get_prime(n))
 * Range:     i8 n <= 31, i16 n <= 3550, i32 n <= 10000, i64 n <= 10000,
 *            i128 n <= 10000, big any n
 * Helpers:   digit_product, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/digit_product.h"

seq_t a(idx_t n)
{
    /* Offset is 1; the behaviour for n = 0 is irrelevant. */
    if (n == 0) return (seq_t)0;

    /* Obtain the n-th prime (1‑based). */
    uint32_t p = get_prime(n);

    /* Compute the product of its decimal digits. */
    seq_t prod = digit_product((seq_t)p);

    /* Return the required difference. */
    return (seq_t)n - prod;
}
