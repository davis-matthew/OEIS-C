/*
 * A163292  a(n) = n-th prime minus (number of digits of n-th prime written in
 *          base 4).
 * https://oeis.org/A163292
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = p_n - digits_base4(p_n) where p_n is the n-th prime and
 *            digits_base4(k) is the number of base‑4 digits of k
 * Range:     u8 n <= 55, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"   /* uint32_t get_prime(idx_t k) */

seq_t a(idx_t n)
{
    /* Get the n‑th prime (fits in 32 bits for the required range). */
    uint64_t prime = (uint64_t)get_prime(n);

    /* Count its digits in base 4. */
    uint64_t tmp = prime;
    uint64_t digits = 0;
    while (tmp) {
        tmp /= 4;
        digits++;
    }

    /* Compute the sequence value.  It is cast to seq_t; if the result
       exceeds the size of seq_t it will overflow as per the
       implementation’s integer type limits. */
    return (seq_t)(prime - digits);
}
