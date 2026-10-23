/*
 * A138677  Prime(n)^8 mod prime(n-1).
 * https://oeis.org/A138677
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n)^8 mod prime(n-1)
 * Range:     u8 n <= 54, u16 n <= 65, u32 n <= 65, u64 n <= 65, u128 n <= 65,
 *            big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Sequence is defined only for n >= 2 (offset 2). */
    if (n < 2) {
        return (seq_t)0;
    }

    /* Retrieve the required primes. */
    seq_t prime_n     = (seq_t)get_prime(n);
    seq_t prime_prev  = (seq_t)get_prime(n - 1);

    /* Compute prime_n^8 modulo prime_prev. */
    return mod_pow(prime_n, 8, prime_prev);
}
