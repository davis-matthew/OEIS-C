/*
 * A038891  Odd primes p such that 19 is a square mod p.
 * https://oeis.org/A038891
 *
 * Offset:    1    (a(1) is the first term)
 * Keywords:  nonn
 * Formula:   not stated
 * Range:     u8 n <= 27, u16 n <= 1000, u32 n <= 1000, u64 n <= 1000,
 *            u128 n <= 1000, big any n
 * Helpers:   is_prime, jacobi_symbol
 *
 * Sequence data from the OEIS (https://oeis.org), licensed CC BY-SA 4.0.
 */

/* Sequence A038891: odd primes p such that 19 is a quadratic residue modulo p. */
/* No closed‑form formula is known; the function generates terms by testing successive prime numbers */
/* and checking the Legendre symbol (19|p)=1. */

#include <stdbool.h>
#include <stdlib.h>
#include <stdint.h>
#include "../utils/jacobi_symbol.h" /* int jacobi_symbol(uint64_t a, uint64_t n) */
#include "../utils/is_prime.h"     /* bool is_prime(idx_t x) */

seq_t a(idx_t n)
{
    /* Storage for terms that have already been found. */
    static seq_t *cache = NULL;          /* array of accepted primes */
    static idx_t cache_len = 0;          /* number of terms stored */
    static idx_t cache_cap = 0;          /* allocated capacity */
    static idx_t next_candidate = 3;     /* next odd integer to test */

    if (n < 1)
        return 0;                       /* undefined for n < 1 */

    /* Expand the cache until we have at least n terms. */
    while (cache_len < n)
    {
        /* Only test odd numbers. */
        while (1)
        {
            if (next_candidate < 3)
                next_candidate = 3;

            if (is_prime((idx_t)next_candidate))
            {
                bool accept = false;
                if (next_candidate == 19ULL)          /* 19 is 0² mod 19 */
                    accept = true;
                else
                    accept = (jacobi_symbol(19ULL, next_candidate) == 1);

                if (accept)
                {
                    /* Ensure capacity. */
                    if (cache_len >= cache_cap)
                    {
                        size_t new_cap = cache_cap ? cache_cap * 2 : 64;
                        seq_t *tmp = (seq_t *)realloc(cache, new_cap * sizeof(seq_t));
                        if (!tmp)
                        {
                            /* Allocation failed. */
                            abort();
                        }
                        cache = tmp;
                        cache_cap = new_cap;
                    }
                    cache[cache_len++] = (seq_t)next_candidate;
                }
            }

            next_candidate += 2;          /* move to the next odd candidate */

            if (cache_len >= n)
                break;                     /* enough terms found */
        }
    }

    return cache[n - 1];
}
