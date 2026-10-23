/*
 * A167771  Twice-isolated primes: primes p such that neither p+-2 nor p+-4 is
 *          prime.
 * https://oeis.org/A167771
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   a(n) is the n-th prime p such that neither p-2 nor p-4 nor p+2
 *            nor p+4 is prime (the “twice‑isolated” primes)
 * Range:     u8 n <= 7, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   append_candidate, get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

#include <stddef.h>
#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"
#include "../utils/append_candidate.h"

seq_t a(idx_t n)
{
    /* Static variables keep state between calls. */
    static seq_t *cache       = NULL;   /* array of found twice‑isolated primes */
    static size_t cache_len   = 0;        /* number of primes stored */
    static size_t cache_cap   = 0;        /* allocated capacity of the array   */
    static idx_t next_prime_i = 1;        /* next prime index to test          */

    /* Generate primes until we have at least n terms cached. */
    while (cache_len < n)
    {
        /* Get the next prime value.  get_prime(1) == 2, etc. */
        uint32_t pk    = get_prime(next_prime_i++);
        seq_t prime    = (seq_t)pk;

        /* Test whether the prime is twice‑isolated. */
        bool isolated = true;
        if (prime > (seq_t)2 && is_prime_big(prime - (seq_t)2))
            isolated = false;
        if (prime > (seq_t)4 && is_prime_big(prime - (seq_t)4))
            isolated = false;
        if (is_prime_big(prime + (seq_t)2))
            isolated = false;
        if (is_prime_big(prime + (seq_t)4))
            isolated = false;

        /* Append to the cache if the condition holds. */
        if (isolated)
            append_candidate(&cache, &cache_len, &cache_cap, prime);
    }

    /* Return the requested element (indices are 0‑based in C). */
    return cache[(size_t)n - 1];
}
