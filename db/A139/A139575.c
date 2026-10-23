/*
 * A139575  Numbers with 47 divisors.
 * https://oeis.org/A139575
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n)^46
 * Range:     u8 none, u16 none, u32 none, u64 n <= 1, u128 n <= 3, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* where prime(n) is the n-th prime number (OEIS A000040) */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* Get the n-th prime as seq_t */
    seq_t p = (seq_t) get_prime(n);
    /* Compute p^46 by repeated multiplication */
    seq_t result = 1;
    for (idx_t i = 0; i < 46; ++i)
        result *= p;
    return result;
}
