/*
 * A135103  Digital sum (base the n-th prime) of n^3.
 * https://oeis.org/A135103
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, base
 * Formula:   a(n) = ds_prime(n)(n^3) (digital sum of n³ in base the nth
 *            prime)
 * Range:     u8 n <= 7, u16 n <= 40, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* retrieve the nth prime (1‑based) */
    uint32_t p = get_prime(n);

    /* compute n³ in the exact arithmetic type seq_t */
    seq_t value = (seq_t)n * (seq_t)n * (seq_t)n;

    /* digital sum in base p */
    seq_t sum = 0;
    while (value > 0) {
        sum += value % (seq_t)p;
        value /= (seq_t)p;
    }

    return sum;
}
