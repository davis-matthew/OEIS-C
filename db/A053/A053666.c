/*
 * A053666  Product of digits of n-th prime.
 * https://oeis.org/A053666
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = product of decimal digits of the nth prime
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   digit_product, get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/digit_product.h"

/* Return the product of the decimal digits of the nth prime.
   n is 1‑based (a(1) = 2). */
seq_t a(idx_t n)
{
    /* The OEIS offset is 1, so n >= 1. */
    if (n < 1) return 0;
    /* Retrieve the n‑th prime.  get_prime returns a uint32_t. */
    seq_t p = (seq_t)get_prime(n);
    /* Compute and return the digit product. */
    return digit_product(p);
}
