/*
 * A141910  Primes congruent to 6 mod 23.
 * https://oeis.org/A141910
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 2, u16 n <= 291, u32 n <= 196, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the nth prime p with p ≡ 6 (mod 23).  The first term (n=1) is 29. */
/* Uses a simple trial‑division primality test from utilities. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t primes_cap = 0;
    static size_t primes_len = 0;
    static seq_t candidate = 6;          /* next number ≡ 6 (mod 23) to test */

    while (primes_len < (size_t)n)
    {
        /* Find the next prime ≡ 6 (mod 23). */
        while (true)
        {
            if (candidate < (seq_t)2) {          /* numbers < 2 are not prime */
                candidate += (seq_t)23;
                continue;
            }
            if (is_prime_big(candidate))
                break;
            candidate += (seq_t)23;
        }

        /* Store the prime. */
        if (primes_len == primes_cap)
        {
            size_t new_cap = primes_cap ? (primes_cap << 1) : 4;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            if (!tmp) abort();                    /* out‑of‑memory */
            primes = tmp;
            primes_cap = new_cap;
        }
        primes[primes_len++] = candidate;

        /* Advance to the next candidate. */
        candidate += (seq_t)23;
    }

    return primes[(size_t)(n - 1)];          /* 1‑based indexing */
}
