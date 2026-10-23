/*
 * A138434  a(n) = ((n-th prime)^5-(n-th prime)^3)/4.
 * https://oeis.org/A138434
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n)^5 - (p_n)^3)/4
 * Range:     u8 n <= 2, u16 n <= 5, u32 n <= 29, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/*    where p_n is the n-th prime (p_1 = 2) */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    /* get the n-th prime (1‑based) */
    uint32_t p_uint = get_prime(n);
    if (p_uint == 0)   /* sanity check: no such prime (should not happen) */
        return 0;

    /* Special case for the first prime 2 where the factorisation above
     * is not integer‑divisible by 4.  Compute directly. */
    if (p_uint == 2)
    {
        seq_t p = 2;
        seq_t p3 = p * p * p;
        seq_t p5 = p3 * p * p;
        return (p5 - p3) / (seq_t)4;
    }

    seq_t p  = (seq_t)p_uint;   /* cast to seq_t once to avoid repeated casts */
    seq_t p2 = p * p;           /* p^2  */
    seq_t p3 = p2 * p;          /* p^3  */
    seq_t b  = (p2 - (seq_t)1) / (seq_t)4;  /* (p^2 - 1)/4 for odd primes */
    return p3 * b;              /* (p^3 * (p^2 - 1))/4 equals desired value */
}
