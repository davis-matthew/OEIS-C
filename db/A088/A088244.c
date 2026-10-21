/*
 * A088244  Number of 3-smooth divisors of n!.
 * https://oeis.org/A088244
 *
 * Offset:    0    (a(0) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = (v2(n!) + 1) * (v3(n!) + 1) where v2 and v3 are the
 *            exponents of 2 and 3 in n!. This uses Legendre's formula to
 *            compute the exponents
 * Range:     u8 n <= 25, u16 n <= 367, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>

seq_t a(idx_t n)
{
    /* compute exponent of 2 in n! */
    seq_t e2 = 0;
    idx_t pow = 2;
    while (pow <= n) {
        e2 += (seq_t)(n / pow);
        if (pow > n / 2) break;   /* prevent overflow of pow <<= 1 */
        pow <<= 1;
    }

    /* compute exponent of 3 in n! */
    seq_t e3 = 0;
    pow = 3;
    while (pow <= n) {
        e3 += (seq_t)(n / pow);
        if (pow > n / 3) break;   /* prevent overflow of pow *= 3 */
        pow *= 3;
    }

    return (e2 + 1) * (e3 + 1);
}
