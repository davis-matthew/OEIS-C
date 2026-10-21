/*
 * A208575  Product of digits of n in factorial base.
 * https://oeis.org/A208575
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = product of the digits of n written in factorial base
 *            (with digits 0..k for the k-th position)
 * Range:     u8 n <= 2758, u16 n <= 9637, u32 n <= 9637, u64 n <= 9637,
 *            u128 n <= 9637, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n) {
    /* Special case n = 0: the empty representation yields product 0. */
    if (n == 0) return (seq_t)0;

    seq_t prod = 1;
    idx_t m = n;     /* mutable copy for division */
    idx_t base = 2;  /* first divisor corresponds to position 1 (base 2) */

    while (m > 0) {
        idx_t digit = m % base;
        m /= base;
        prod *= (seq_t)digit;   /* multiplication in seq_t */
        if (prod == 0) return prod;  /* product is zero once a digit is zero */
        ++base;
    }
    return prod;
}
