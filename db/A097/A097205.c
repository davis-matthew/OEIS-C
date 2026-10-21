/*
 * A097205  Numbers of the form 3^p * p^3 for p prime.
 * https://oeis.org/A097205
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = 3^{p_n} * p_n^3 (where p_n is the n-th prime)
 * Range:     u8 n <= 1, u16 n <= 3, u32 n <= 6, u64 n <= 11, u128 n <= 15,
 *            big any n
 * Helpers:   get_prime, pow3
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/pow3.h"

seq_t a(idx_t n)
{
    /* Fetch the n-th prime (1-indexed) */
    uint32_t prime = get_prime(n);

    /* Convert to seq_t for arithmetic */
    seq_t p = (seq_t)prime;

    /* Compute 3^prime and multiply by prime^3 */
    seq_t pow3p = pow3(prime);               /* 3^prime */
    seq_t prime_cubed = p * p * p;           /* prime^3 */
    seq_t result = pow3p * prime_cubed;      /* 3^prime * prime^3 */

    return result;
}
