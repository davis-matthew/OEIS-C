/*
 * A240546  a(n) = prime(n+1)^n mod prime(n).
 * https://oeis.org/A240546
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, less
 * Formula:   a(n) = prime(n+1)^n mod prime(n)
 * Range:     u8 n <= 53, u16 n <= 6753, u32 n <= 10000, u64 n <= 10000,
 *            u128 n <= 10000, big any n
 * Helpers:   get_prime, mod_pow
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include "../utils/get_prime.h"
#include "../utils/mod_pow.h"

seq_t a(idx_t n)
{
    /* Use 1‑based offset: a(1) is defined. */
    if (n == 0)
        return 0; /* undefined for n == 0, but safe return */

    /* Retrieve the nth and (n+1)th primes. */
    uint32_t p_n   = get_prime(n);     /* prime(n)   */
    uint32_t p_np1 = get_prime(n + 1); /* prime(n+1) */

    /* Compute (prime(n+1))^n modulo prime(n). */
    return mod_pow((seq_t)p_np1, (uint64_t)n, (seq_t)p_n);
}
