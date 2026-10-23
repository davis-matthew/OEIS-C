/*
 * A128212  a(n) = Sum_digits(p), where p is the product of the digits of n.
 * https://oeis.org/A128212
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = sum_digits(product_of_digits(n))
 * Range:     u8 n <= 6000, u16 n <= 6000, u32 n <= 6000, u64 n <= 6000,
 *            u128 n <= 6000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include <stddef.h>

seq_t a(idx_t n)
{
    /* a(1) is the first term. Guard against n == 0, though
       the OEIS offset is 1. */
    if (n == 0) return 0;

    /* Compute the product of the decimal digits of n.  The product
       for n ≤ 10^4 fits in 32 bits, so a 64‑bit unsigned integer is
       safe for all builds. */
    uint64_t prod = 1;
    uint64_t tmp = (uint64_t)n;

    while (tmp > 0) {
        unsigned d = (unsigned)(tmp % 10);
        tmp /= 10;
        if (d == 0) {          /* product becomes zero immediately */
            prod = 0;
            break;
        }
        prod *= d;
    }

    /* Sum the decimal digits of the product. */
    unsigned sum = 0;
    if (prod == 0) {
        return (seq_t)0;
    }
    while (prod > 0) {
        sum += (unsigned)(prod % 10);
        prod /= 10;
    }

    return (seq_t)sum;
}
