/*
 * A160668  Distance between prime(n) and the next higher power of 10.
 * https://oeis.org/A160668
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn, base
 * Formula:   a(n) = 10^d - prime(n), where d = number of decimal digits of
 *            prime(n)
 * Range:     u8 n <= 25, u16 n <= 1229, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   digits, get_prime, pow10
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Compute the nth term of OEIS A160668: distance from the nth prime to the next higher power of 10. */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/digits.h"
#include "../utils/pow10.h"

seq_t a(idx_t n)
{
    /* Get the nth prime (1‑based). */
    uint32_t p = get_prime(n);

    /* Number of decimal digits of the prime. */
    int d = digits((idx_t)p);

    /* 10^d as a seq_t value. */
    seq_t power = pow10((idx_t)d);

    /* Difference: 10^d - p. */
    seq_t diff = power - (seq_t)p;

    return diff;
}
