/*
 * A090938  Least multiple of n == 1 (mod prime(n)).
 * https://oeis.org/A090938
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 11, u16 n <= 113, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* a(n) = n * inv(n, prime(n))   where inv(a,b) = a^(-1) mod b */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* nth prime number */
    uint32_t p = get_prime(n);

    /* modular inverse of n modulo p = n^(p-2) mod p   (Fermat's little theorem) */
    seq_t inv = mod_pow((seq_t)n, (uint64_t)(p - 2), (seq_t)p);

    /* least multiple of n that is 1 (mod prime(n)) */
    return (seq_t)n * inv;
}
