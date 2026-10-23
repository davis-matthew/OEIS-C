/*
 * A138676  a(n) = prime(n)^7 mod prime(n-1).
 * https://oeis.org/A138676
 *
 * Offset:    2    (a(2) is the first term)
 * Keywords:  nonn, look
 * Formula:   a(n) = prime(n)^7 mod prime(n-1)
 * Range:     u8 n <= 57, u16 n <= 6734, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Sequence is defined for n ≥ 2. For n < 2 we return 0. */
    if (n < 2) return 0;

    /* Obtain the (n-1)-th and n-th primes.  get_prime(1) is 2. */
    uint32_t p_nm1 = get_prime(n - 1);
    uint32_t p_n   = get_prime(n);

    /* Compute p_n^7 mod p_nm1 using the provided modular exponentiation. */
    seq_t result = mod_pow((seq_t)p_n, (uint64_t)7, (seq_t)p_nm1);

    return result;
}
