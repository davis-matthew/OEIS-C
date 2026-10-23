/*
 * A141896  Primes congruent to 17 mod 21.
 * https://oeis.org/A141896
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   not stated
 * Range:     u8 n <= 4, u16 n <= 542, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Computes the n-th prime ≡ 17 (mod 21) by successive testing of candidates. */
#include <stdlib.h>
#include <stdbool.h>
#include "../utils/is_prime.h"

seq_t a(idx_t n)
{
    static seq_t *primes = NULL;
    static size_t cap = 0;
    static size_t sz = 0;
    static idx_t k = 0;           /* number of candidates checked */

    while (sz < n)
    {
        /* next candidate congruent to 17 modulo 21 */
        idx_t candidate = 17 + 21 * k;
        ++k;

        if (!is_prime(candidate))
            continue;

        /* grow the cache if necessary */
        if (sz >= cap)
        {
            size_t new_cap = cap == 0 ? 64 : cap * 2;
            if (n > new_cap)
                new_cap = n;
            seq_t *tmp = (seq_t *)realloc(primes, new_cap * sizeof(seq_t));
            primes = tmp;
            cap = new_cap;
        }

        primes[sz++] = (seq_t)candidate;
    }

    return primes[n - 1];
}
