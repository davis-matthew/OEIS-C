/*
 * A060234  a(n) = (prime(n) mod (prime(n+1)-prime(n))).
 * https://oeis.org/A060234
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) = prime(n) mod (prime(n+1) - prime(n))
 * Range:     u8 n <= 54, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   get_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"

seq_t a(idx_t n)
{
    if (n < 1)               /* offset starts at 1 */
        return 0;

    /* Retrieve the required consecutive primes */
    uint32_t p_cur   = get_prime(n);
    uint32_t p_next  = get_prime(n + 1);
    uint32_t diff    = p_next - p_cur;   /* gap between primes */

    /* Compute the residue modulo the prime gap */
    seq_t result = ((seq_t)p_cur) % diff;
    return result;
}
