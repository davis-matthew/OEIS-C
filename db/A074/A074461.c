/*
 * A074461  Average digit (rounded down) in the decimal expansion of the n-th
 *          prime number.
 * https://oeis.org/A074461
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = floor( digit_sum( prime(n) ) / number_of_digits( prime(n)
 *            ) )
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* prime(n) is the n‑th prime (OEIS A000040) and digit_sum/number_of_digits are taken in base 10. */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* obtain the nth prime */
    seq_t prime = get_prime(n);

    /* sum of decimal digits and digit count */
    seq_t sum  = 0;
    seq_t cnt  = 0;
    seq_t temp = prime;

    /* iterate over decimal digits */
    while (temp > 0) {
        sum += temp % 10;   /* add least significant digit */
        temp /= 10;         /* shift right */
        cnt++;              /* increment digit counter */
    }

    /* average rounded down */
    return sum / cnt;
}
