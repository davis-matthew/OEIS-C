/*
 * A171048  In the sequence of prime numbers, replace all the '8' digits with
 *          '7' and vice versa.
 * https://oeis.org/A171048
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base, easy
 * Formula:   a(n) = transform the n-th prime by swapping 7 and 8 digits
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n) {
    /* Obtain the n-th prime (1‑based indexing). */
    uint32_t prime = get_prime(n);

    seq_t result = 0;
    seq_t mul = 1;
    while (prime > 0) {
        unsigned digit = prime % 10;
        if (digit == 7u)          digit = 8u;
        else if (digit == 8u)     digit = 7u;

        result = result + mul * (seq_t)digit;
        mul = mul * 10;
        prime /= 10;
    }
    return result;
}
