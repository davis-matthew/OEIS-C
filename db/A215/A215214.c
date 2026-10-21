/*
 * A215214  Primes congruent to {0, 1, 2, 5} mod 11.
 * https://oeis.org/A215214
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn, easy
 * Formula:   a(n) = the n‑th prime congruent to 0, 1, 2, or 5 modulo 11
 * Range:     u8 n <= 16, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   grow_seq, is_prime_big
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* The function searches lazily and memoises found primes. */

#include <stddef.h>
#include "../utils/grow_seq.h"
#include "../utils/is_prime_big.h"

seq_t a(idx_t n)
{
    /* static cache of found primes */
    static seq_t  *cache = NULL;
    static size_t  cap   = 0;   /* current allocation size */
    static size_t  sz    = 0;   /* number of primes stored */
    static seq_t   last  = 0;   /* last prime stored */

    /* compute primes until we have at least n of them */
    while (sz < (size_t)n)
    {
        seq_t cand = last + 1;   /* start searching from next integer */

        /* find the next prime with required residue class */
        while (1)
        {
            if (is_prime_big(cand))
            {
                seq_t rem = cand % 11;
                if (rem == 0 || rem == 1 || rem == 2 || rem == 5)
                    break;
            }
            cand = cand + 1;
        }

        /* grow the cache if necessary */
        grow_seq(&cache, &cap, sz + 1);

        cache[sz] = cand;
        ++sz;
        last = cand;
    }

    return cache[n - 1];
}
