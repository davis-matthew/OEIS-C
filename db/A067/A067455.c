/*
 * A067455  Let m be the product of the decimal digits in n, then a(n) = 0 if
 *          m = 0, otherwise a(n) = n!/m.
 * https://oeis.org/A067455
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  base, easy, nonn
 * Formula:   a(n) = 0 if the product of the decimal digits of n is 0;
 *            otherwise a(n) = n! / prod_digits(n). (sequence offset 1 – a(1)
 *            is the first listed term)
 * Range:     u8 n <= 5, u16 n <= 8, u32 n <= 12, u64 n <= 20, u128 n <= 24,
 *            big any n
 * Helpers:   none
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>

seq_t a(idx_t n)
{
    /* Handle the special case n = 0 (not used in the OEIS offset but safe). */
    if (n == 0)
        return (seq_t)0;

    /* Compute the product of the decimal digits of n.
       Return 0 immediately if any digit is 0. */
    uint64_t prod = 1;          /* works for any idx_t because digit < 10 */
    idx_t tmp = n;
    while (tmp > 0) {
        uint64_t d = tmp % 10;
        if (d == 0u)
            return (seq_t)0;
        prod *= d;
        tmp /= 10;
    }

    /* Compute n! iteratively in seq_t. */
    seq_t fact = 1;
    for (idx_t i = 2; i <= n; ++i)
        fact *= (seq_t)i;

    /* Divide by the digit product (fits in seq_t because prod < 10^9). */
    return fact / (seq_t)prod;
}
