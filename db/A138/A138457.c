/*
 * A138457  a(n) = ((n-th prime)^6-(n-th prime)^4)/8.
 * https://oeis.org/A138457
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = ((p_n)^6-(p_n)^4)/8 where p_n is the n-th prime
 * Range:     u8 n <= 2, u16 n <= 4, u32 n <= 16, u64 n <= 342, u128 n <= 1000,
 *            big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    uint32_t np = get_prime(n);          /* nth prime, 1‑based */
    if (np == 2)                          /* special case for the first prime */
        return (seq_t)6;                 /* (2^6-2^4)/8 = 48/8 = 6 */

    seq_t p  = (seq_t)np;
    seq_t p2 = p * p;                    /* p² */
    /* For odd primes, (p²-1) is divisible by 8 */
    seq_t r   = (p2 - 1) / 8;             /* (p²-1)/8 */
    seq_t tmp = p2 * r;                   /* p² * ((p²-1)/8) */
    return tmp * p2;                     /* result fits in seq_t when a(n) fits */
}
