/*
 * A136035  Remainder when dividing 2^q - 1 by q + 1 where q is the n-th
 *          prime.
 * https://oeis.org/A136035
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   a(n) = (2^prime(n) - 1) mod (prime(n)+1)
 * Range:     u8 n <= 67, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stdint.h>
#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* nth prime */
    uint32_t p = get_prime(n);
    /* modulus = prime + 1 */
    seq_t mod = (seq_t)(p + 1);
    /* 2^p mod (p+1) */
    seq_t r = mod_pow((seq_t)2, (uint64_t)p, mod);
    /* return (2^p - 1) mod (p+1) */
    if (r == 0)
        return (seq_t)p; /* (mod - 1) == p */
    return r - (seq_t)1;
}
