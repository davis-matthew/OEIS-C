/*
 * A053089  a(n) = prime(n)^prime(n+1).
 * https://oeis.org/A053089
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = prime(n) ^ prime(n+1)
 * Range:     u8 n <= 2, u16 n <= 2, u32 n <= 4, u64 n <= 6, u128 n <= 8,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include <stdint.h>

/* Exponentiation by squaring for arbitrary seq_t types.
   Works for both fixed-width integer types and arbitrary-precision
   integer classes provided the multiplication operator is defined. */
static seq_t pow_seq_t(seq_t base, idx_t exp)
{
    seq_t result = (seq_t)1;
    while (exp > 0)
    {
        if (exp & 1)
            result *= base;
        exp >>= 1;
        if (exp)
            base *= base;
    }
    return result;
}

seq_t a(idx_t n)
{
    /* Retrieve the nth and (n+1)th primes. */
    uint32_t p1 = get_prime(n);     /* prime(n)   */
    uint32_t p2 = get_prime(n + 1); /* prime(n+1) */

    seq_t base = (seq_t)p1;
    idx_t exp  = (idx_t)p2;

    return pow_seq_t(base, exp);
}
