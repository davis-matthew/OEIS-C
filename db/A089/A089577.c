/*
 * A089577  a(n) = r-th prime of the form (p-q)/(q-r) with r=prime(n+1),
 *          q=prime(n+2), and primes p > q.
 * https://oeis.org/A089577
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  easy, nonn
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 28, u32 n <= 28, u64 n <= 66, u128 n <= 66,
 *            big any n
 * Helpers:   get_prime, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Search: For each n ≥ 1 let r = prime(n+1), q = prime(n+2).
   Find the r‑th prime k such that p = q + (q–r)·k is also prime.
   The sequence term a(n) is this prime k. */

#include <stdint.h>
#include <stdbool.h>
#include "../utils/get_prime.h"
#include "../utils/is_prime_big.h"

/* a(n) = r-th prime k with p = q + (q-r)·k prime,
   where r = prime(n+1), q = prime(n+2) */
seq_t a(idx_t n)
{
    /* Locate r and q */
    uint32_t r = get_prime((idx_t)n + 1);   /* (n+1)-th prime */
    uint32_t q = get_prime((idx_t)n + 2);   /* (n+2)-th prime */

    /* d = q - r, guaranteed positive */
    seq_t d = (seq_t)q - (seq_t)r;

    /* Search through primes k */
    idx_t found = 0;           /* number of suitable primes encountered */
    idx_t k_index = 1;         /* primes start at index 1 → 2 */
    while (true)
    {
        uint32_t k = get_prime(k_index);                /* current prime k */
        seq_t p = (seq_t)q + d * (seq_t)k;              /* p = q + d*k */

        if (is_prime_big(p))                           /* check p prime */
        {
            found++;
            if (found == (idx_t)r)                     /* r-th such prime */
                return (seq_t)k;
        }
        k_index++;                                      /* next prime k */
    }
}
